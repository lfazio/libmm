# libmm

This lib is intendend to work even on embedde RTOS or baremetal to track memory allocation on the heap.

## Debug

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