# SPDX Licence-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

gtest_dep = dependency('gtest', required : true)

test_alloc = executable('test_alloc',
  'test_alloc.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('alloc_test', test_alloc)

test_alloc_track = executable('test_alloc_track',
  'test_alloc_track.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('alloc_track_test', test_alloc_track)

test_string = executable('test_string',
  'test_string.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('string_test', test_string)

test_slab = executable('test_slab',
  'test_slab.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('slab_test', test_slab)

test_slab_invalid = executable('test_slab_invalid',
  'test_slab_invalid.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('slab_test_invalid', test_slab_invalid)

test_slab_static = executable('test_slab_static',
  'test_slab_static.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('slab_test_static', test_slab_static)

test_slab_arena = executable('test_slab_arena',
  'test_slab_arena.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('slab_test_arena', test_slab_arena)

test_rbi = executable('test_rbi',
  'test_rbi.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('rbi_test', test_rbi)

test_rb = executable('test_rb',
  'test_rb.cpp',
  dependencies: [gtest_dep, libmm_dep]
)
test('rb_test', test_rb)
