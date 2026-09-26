# Building DeSmuME for hdrv (Headless Runtime Driver)

The headless runtime driver (`hdrv`) links against DeSmuME's static library
(`libdesmume.a`) to step frames without a GUI.

## Prerequisites

Install development packages (Fedora/Ubuntu equivalents noted):

| Package | Fedora | Ubuntu/Debian |
|---------|--------|---------------|
| C++17 compiler | `gcc-c++` | `g++` |
| SDL2 | `SDL2-devel` | `libsdl2-dev` |
| GLib 2.0 | `glib2-devel` | `libglib2.0-dev` |
| OpenAL | `openal-soft-devel` | `libopenal-dev` |
| SoundTouch | `soundtouch-devel` | `libsoundtouch-dev` |
| zziplib | `zziplib-devel` | `libzzip-dev` |
| zlib | `zlib-devel` | `zlib1g-dev` |
| libpcap | `libpcap-devel` | `libpcap-dev` |
| OpenGL/GLU | `mesa-libGL-devel mesa-libGLU-devel` | `libgl-dev libglu1-mesa-dev` |
| Lua 5.1 | `lua-devel` | `liblua5.1-0-dev` |

## Acquire DeSmuME 0.9.13

```sh
git clone --depth 1 --branch release_0_9_13 \
    https://github.com/TASVideo/desmume.git /tmp/desmume
```

## Build the static library

```sh
cd /tmp/desmume/desmume/src/frontend/posix
./configure --disable-glade --disable-gtktest
make -j$(nproc) libdesmume.a
```

The library is at `src/frontend/posix/libdesmume.a`.

## Build hdrv

```sh
scripts/build-hdrv.sh /tmp/desmume/desmume /tmp/hdrv
```

Or with an explicit output path:

```sh
scripts/build-hdrv.sh /tmp/desmume/desmume tools/hdrv
```

The build script uses `pkg-config` for SDL2 and GLib flags when available,
falling back to standard include paths. If your system uses non-standard
locations, set `PKG_CONFIG_PATH` or override `CFLAGS`/`LIBS` before running.

## Troubleshooting

**Missing `libdesmume.a`**: Run `make libdesmume.a` in the posix frontend dir.

**Missing headers**: Install the corresponding `-devel` / `-dev` package listed
above.

**pkg-config not finding SDL2**: Try `export PKG_CONFIG_PATH=/usr/lib64/pkgconfig`
or install `pkgconf`.

**Linker errors for OpenAL/SoundTouch**: These are optional for hdrv. If
unavailable, remove `-lopenal` and `-lSoundTouch` from the build command.
