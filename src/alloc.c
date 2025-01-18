// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#define _GNU_SOURCE
#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <freebsd/sys/sys/queue.h>

#include <mm/config/cdefs.h>
#include <mm/config/config.h>
#include <mm/config/critical.h>
#include <mm/config/panic.h>
#include <mm/config/thread.h>

#include <mm/track.h>
#include <mm/alloc.h>

/* --------------------------------------------------------------------------
 * LOCAL CONSTANTS
 * -------------------------------------------------------------------------- */

#define MEMTRACK_MAGIC 0x4d454d54 /* MEMT in ascii */
#define MEMTRACK_MAGIC_FREE 0x46524545 /* FREE in ascii */

/* --------------------------------------------------------------------------
 * LOCAL TYPES
 * -------------------------------------------------------------------------- */

struct _mt_info {
	TAILQ_ENTRY(_mt_info) link;
	uint32_t size;
	const char *file;
	int line;
	int tid;
	uint32_t magic;
};

struct _by_thread {
	int tid; /*!< Thread ID */
	size_t allocated; /*!< Curent heap usage per thread */
	size_t max_allocated; /*!< Maximum heap usage per thread */

	TAILQ_ENTRY(_by_thread) link;
};

struct _mm_ctx {
	struct {
		bool enable; /*!< Enable memory tracking */
		time_t period; /*!< Period between data retrieval */
		TAILQ_HEAD(mm_chunks, _mt_info)
		chunks; /*!< Related memory chunks information */
		size_t allocated; /*!< Curent heap usage */
		size_t max_allocated; /*!< Maximum heap usage */
		size_t count; /*!< Number of allocated chunks */

		int key; /*!< Memory tracking thread key; */

		TAILQ_HEAD(mm_thread_chunks, _by_thread)
		by_thread; /*!< List of thread heap usage (thread specific storage list) */
	} memtrack;
};

/* --------------------------------------------------------------------------
 * LOCAL VARIABLES
 * -------------------------------------------------------------------------- */

static struct _mm_ctx _ctx;

/* --------------------------------------------------------------------------
 * LOCAL CONSTANTS
 * -------------------------------------------------------------------------- */

#define MT_INFO_SIZE sizeof(struct _mt_info)
#define MT_INFO_SIZE_ALIGNED ROUNDUP(MT_INFO_SIZE, MM_ALIGN)
#define MT_GET_METADATA(ptr) \
	((struct _mt_info *)((uintptr_t)ptr - MT_INFO_SIZE_ALIGNED))
#define MT_GET_DATA(ptr) ((void *)((uintptr_t)ptr + MT_INFO_SIZE_ALIGNED))

/* --------------------------------------------------------------------------
 * LOCAL FUNCTIONS
 * -------------------------------------------------------------------------- */

static void _thread_clear(void *ptr)
{
	struct _by_thread *ts = ptr;
	uint32_t flags;

	if (!ts)
		return;

	flags = IRQ_SAVE();
	TAILQ_REMOVE(&_ctx.memtrack.by_thread, ts, link);
	IRQ_RESTORE(flags);

	free(ts);
}

static void _thread_alloc(struct _mm_ctx *ctx, size_t size)
{
	struct _by_thread *ts;
	uint32_t flags;

	if (!ctx || ctx->memtrack.key <= 0)
		return;

	ts = THREAD_GETSPECIFIC(ctx->memtrack.key);
	if (!ts) {
		ts = calloc(1, sizeof(struct _by_thread));
		if (THREAD_SETSPECIFIC(ctx->memtrack.key, ts) < 0) {
			free(ts);
			ts = NULL;
		} else {
			if (ts) {
				ts->tid = THREAD_GETTID();

				flags = IRQ_SAVE();
				TAILQ_INSERT_TAIL(&ctx->memtrack.by_thread, ts,
						  link);
				IRQ_RESTORE(flags);
			}
		}
	}

	if (ts) {
		flags = IRQ_SAVE();
		ts->allocated += size;
		if (ts->allocated > ts->max_allocated)
			ts->max_allocated = ts->allocated;
		IRQ_RESTORE(flags);
	}
}

static void _thread_drop(struct _mm_ctx *ctx, size_t size)
{
	struct _by_thread *ts;

	if (!ctx || ctx->memtrack.key <= 0)
		return;

	ts = THREAD_GETSPECIFIC(_ctx.memtrack.key);
	if (ts)
		ts->allocated -= size;
}

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

#define	__malloc_like	__attribute__((__malloc__(__mm_free, 1)))
void *__mm_malloc(size_t size, const char *file, int line)
{
	return __mm_realloc(NULL, size, file, line);
}

#define	__malloc_like	__attribute__((__malloc__(__mm_free, 1)))
void *__mm_calloc(size_t nmemb, size_t size, const char *file, int line)
{
	void *ptr;
	ptr = __mm_realloc(NULL, nmemb * size, file, line);
	if (ptr)
		memset(ptr, 0, nmemb * size);

	return ptr;
}

void __mm_free(void *ptr, const char *file, int line)
{
	__mm_realloc(ptr, 0, file, line);
}

#define	__malloc_like	__attribute__((__malloc__(__mm_free, 1)))
void *__mm_realloc(void *ptr, size_t size, const char *file, int line)
{
	void *new_ptr;
	struct _mt_info *info = NULL;
	uint32_t flags;
	uint32_t sz;
	uint32_t old_size = 0;
	bool should_count = (ptr == NULL);

	sz = size;
	if (_ctx.memtrack.enable)
		sz += MT_INFO_SIZE_ALIGNED;

	if (ptr && _ctx.memtrack.enable) {
		info = MT_GET_METADATA(ptr);
		if (info->magic == MEMTRACK_MAGIC_FREE)
			PANIC("ptr=%p: double free detected\n", file, line,
			      ptr);

		if (info->magic == MEMTRACK_MAGIC) {
			flags = IRQ_SAVE();
			TAILQ_REMOVE(&_ctx.memtrack.chunks, info, link);
			info->magic = MEMTRACK_MAGIC_FREE;
			IRQ_RESTORE(flags);

			ptr = info;
			old_size = info->size;
		}
	}

	new_ptr = realloc(ptr, sz);

	if (!_ctx.memtrack.enable)
		return new_ptr;

	flags = IRQ_SAVE();
	_ctx.memtrack.allocated += (size - old_size);
	if (_ctx.memtrack.allocated > _ctx.memtrack.max_allocated)
		_ctx.memtrack.max_allocated = _ctx.memtrack.allocated;
	if (!size) {
		_ctx.memtrack.count -= 1;
		IRQ_RESTORE(flags);

		if (info)
			_thread_drop(&_ctx, info->size);

		return new_ptr;
	}
	IRQ_RESTORE(flags);

	_thread_alloc(&_ctx, size - old_size);

	info = new_ptr;
	info->magic = MEMTRACK_MAGIC;
	info->size = size;
	info->line = line;
	info->file = file;
	info->tid = THREAD_GETTID();

	flags = IRQ_SAVE();
	TAILQ_INSERT_TAIL(&_ctx.memtrack.chunks, info, link);
	if (should_count)
		_ctx.memtrack.count++;
	IRQ_RESTORE(flags);

	return (struct _mt_info *)MT_GET_DATA(new_ptr);
}

int mm_mt_activate(void)
{
	int err;

	_ctx.memtrack.allocated = 0;
	_ctx.memtrack.max_allocated = 0;

	TAILQ_INIT(&_ctx.memtrack.chunks);
	TAILQ_INIT(&_ctx.memtrack.by_thread);

	err = THREAD_KEY_CREATE(&_ctx.memtrack.key, _thread_clear);
	if (err == -ENOSYS)
		_ctx.memtrack.key = -1;
	else if (err < 0)
		return err;

	_ctx.memtrack.enable = true;

	return 0;
}

void mm_mt_deactivate(void)
{
	_ctx.memtrack.enable = false;
}

int mm_mt_summary(bool verbose, int (*_puts)(void *ctx, const char *str),
		  void *ctx)
{
	struct _mt_info *info, *next;
	char txt[256];

	if (!_puts)
		return -EINVAL;

	if (!_ctx.memtrack.enable)
		return -ENOSYS;

	_puts(ctx, "{\n");
	snprintf(txt, sizeof(txt), "\t'current-heap-usage': %zu,\n",
		 _ctx.memtrack.allocated);
	_puts(ctx, txt);
	snprintf(txt, sizeof(txt), "\t'max-heap-usage': %zu,\n",
		 _ctx.memtrack.max_allocated);
	_puts(ctx, txt);

	if (verbose && !TAILQ_EMPTY(&_ctx.memtrack.chunks)) {
		_puts(ctx, "\t'allocations': [\n");
		TAILQ_FOREACH_SAFE(info, &_ctx.memtrack.chunks, link, next) {
			snprintf(txt, sizeof(txt),
				 "\t\t{\n"
				 "\t\t\t'file': '%s',\n"
				 "\t\t\t'line': %d,\n"
				 "\t\t\t'thread': [ '%s', %d ]\n"
				 "\t\t\t'mem': [ %p, %" PRIu32 " ]\n"
				 "\t\t},\n",
				 info->file, info->line,
				 info->tid == -1 ? "main" :
						   THREAD_GET_NAME(info->tid),
				 info->tid, (void *)MT_GET_DATA(info),
				 info->size);
			_puts(ctx, txt);
		}
		_puts(ctx, "\t],\n");
	}

	snprintf(txt, sizeof(txt), "\t'overallocation-per-alloc': %zu,\n",
		 MT_INFO_SIZE_ALIGNED);
	_puts(ctx, txt);
	snprintf(txt, sizeof(txt), "\t'total-overallocation': %zu,\n",
		 _ctx.memtrack.count * MT_INFO_SIZE_ALIGNED);
	_puts(ctx, txt);

	_puts(ctx, "}\n");

	return _ctx.memtrack.allocated;
}

int mm_mt_summary_for_thread(int tid, const char *thread_name, bool verbose,
			     int (*_puts)(void *ctx, const char *str),
			     void *ctx)
{
	struct _mt_info *info, *next;
	struct _by_thread *ts, *tsnext;
	char txt[256];

	if (!_ctx.memtrack.enable)
		return -ENOSYS;

	if (tid < -1)
		return -EINVAL;

	snprintf(txt, sizeof(txt), "{\n\t'thread': [ '%s', %d ]\n",
		 thread_name ? thread_name : THREAD_GET_NAME(tid), tid);
	_puts(ctx, txt);

	/* find thread info */
	TAILQ_FOREACH_SAFE(ts, &_ctx.memtrack.by_thread, link, tsnext) {
		if (ts->tid != tid)
			continue;

		snprintf(txt, sizeof(txt), "\t'current-heap-usage': %zu,\n",
			 ts->allocated);
		_puts(ctx, txt);
		snprintf(txt, sizeof(txt), "\t'maximum-heap-usage': %zu,\n",
			 ts->max_allocated);
		_puts(ctx, txt);

		if (verbose && ts->allocated) {
			_puts(ctx, "\t'allocations': [\n");
			TAILQ_FOREACH_SAFE(info, &_ctx.memtrack.chunks, link,
					   next) {
				if (info->tid != tid)
					continue;

				snprintf(txt, sizeof(txt),
					 "\t\t{\n"
					 "\t\t\t'file': '%s',\n"
					 "\t\t\t'line': %d,\n"
					 "\t\t\t'thread': [ '%s', %d ]\n"
					 "\t\t\t'mem': [ %p, %" PRIu32 " ]\n"
					 "\t\t},\n",
					 info->file, info->line,
					 info->tid == -1 ?
						 "main" :
						 THREAD_GET_NAME(info->tid),
					 info->tid, (void *)MT_GET_DATA(info),
					 info->size);
				_puts(ctx, txt);
			}
			_puts(ctx, "\t],\n");
		}

		_puts(ctx, "}\n");

		return ts->allocated;
	}

	_puts(ctx, "\t'current-heap-usage': 0,\n");
	_puts(ctx, "\t'maximum-heap-usage': 0,\n");
	_puts(ctx, "}\n");
	
	return 0;
}

struct mm_malloc_info mm_malloc_info(void)
{
	struct mm_malloc_info info = {
		.uallocated = _ctx.memtrack.allocated,
		.umaxallocated = _ctx.memtrack.max_allocated,
		.ucount = _ctx.memtrack.count,
	};

	return info;
}