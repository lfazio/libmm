# libmm

This library is intended to work even on embedded RTOS or baremetal projects to track memory allocations on the heap.

This library might also be 

## Build

```sh
meson setup builddir
```

BUILDTYPE can be one of the followings:
- debug
- minsize
- release
- or any other as defined in meson documentation


```sh
meson configure --buildtype ${BUILDTYPE} -C builddir
```

```sh
meson compile -C builddir
```

Run the tests:
```sh
meson test -C builddir
```

## Coverage build

```sh
meson configure -C builddir -Db_coverage=true
```

```sh
meson compile -C builddir
```

Run the tests:
```sh
meson test -C builddir
```
As a dependency you will need at least `gcovr`.

```sh
ninja coverage-html -C builddir/
```
