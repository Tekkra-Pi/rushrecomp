#!/bin/sh
set -eu

# Build the headless DeSmuME runtime driver (hdrv).
#
# Usage:
#   scripts/build-hdrv.sh <desmume-root> [output]
#
# The <desmume-root> should contain src/frontend/posix/libdesmume.a.
# If omitted, [output] defaults to tools/hdrv.
#
# Portability: uses pkg-config where available, falls back to known
# include paths.  Adjust PKG_CONFIG_PATH or CFLAGS/LIBS overrides
# if your system layout differs.

usage() {
    echo "usage: $0 <desmume-0.9.13-root> [output]" >&2
    echo "example: $0 /path/to/desmume-release_0_9_13/desmume /tmp/hdrv" >&2
    exit 2
}

if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
    usage
fi

DESMUME_ROOT="$1"
OUTPUT="${2:-tools/hdrv}"
SRC="$DESMUME_ROOT/src"
LIB="$SRC/frontend/posix/libdesmume.a"

if [ ! -f "$LIB" ]; then
    echo "error: missing $LIB" >&2
    echo "Build the DeSmuME 0.9.13 static library first." >&2
    echo "" >&2
    echo "Quick-start (DeSmuME 0.9.13):" >&2
    echo "  1. git clone --depth 1 --branch release_0_9_13 \\" >&2
    echo "       https://github.com/TASVideo/desmume.git /tmp/desmume" >&2
    echo "  2. cd /tmp/desmume/desmume/src/frontend/posix" >&2
    echo "  3. ./configure --disable-glade --disable-gtktest" >&2
    echo "  4. make -j\$(nproc) libdesmume.a" >&2
    exit 1
fi

# --- Discover compiler flags via pkg-config (preferred) or fallback ---

pkg_cflags() {
    local pc="$1"
    if pkg-config --exists "$pc" 2>/dev/null; then
        pkg-config --cflags "$pc"
    else
        echo ""
    fi
}

pkg_libs() {
    local pc="$1"
    if pkg-config --exists "$pc" 2>/dev/null; then
        pkg-config --libs "$pc"
    else
        echo ""
    fi
}

SDL_CFLAGS=$(pkg_cflags sdl2)
SDL_LIBS=$(pkg_libs sdl2)
GLIB_CFLAGS=$(pkg_cflags glib-2.0 gthread-2.0)
GLIB_LIBS=$(pkg_libs glib-2.0 gthread-2.0)

# Fallback for SDL2 if pkg-config fails
if [ -z "$SDL_CFLAGS" ]; then
    if [ -d /usr/include/SDL2 ]; then
        SDL_CFLAGS="-I/usr/include/SDL2"
    elif [ -d /usr/local/include/SDL2 ]; then
        SDL_CFLAGS="-I/usr/local/include/SDL2"
    fi
fi
if [ -z "$SDL_LIBS" ]; then
    SDL_LIBS="-lSDL2"
fi

# Fallback for glib if pkg-config fails
if [ -z "$GLIB_CFLAGS" ]; then
    GLIB_CFLAGS="-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include"
fi
if [ -z "$GLIB_LIBS" ]; then
    GLIB_LIBS="-lgthread-2.0 -pthread -lglib-2.0"
fi

# Fixed include paths for DeSmuME internal headers
DESMUME_INC="-I$SRC -I$SRC/libretro-common/include -I$SRC/frontend -I$SRC/frontend/interface"

# Optional library headers (warn but don't fail if missing)
EXTRA_CFLAGS=""
EXTRA_LIBS=""

# SoundTouch
if [ -d /usr/include/soundtouch ]; then
    EXTRA_CFLAGS="$EXTRA_CFLAGS -I/usr/include/soundtouch"
fi
# OpenAL
if [ -d /usr/include/AL ]; then
    EXTRA_CFLAGS="$EXTRA_CFLAGS -I/usr/include/AL"
fi
# zzip
if [ -d /usr/include/zzip ]; then
    EXTRA_CFLAGS="$EXTRA_CFLAGS -I/usr/include/zzip"
fi

# Library link order (static archive must come after objects that reference it)
LINK_LIBS="-lSDL2 -lgthread-2.0 -pthread -lglib-2.0"
# Optional libs — include if header dir was found
for libspec in \
    "-lSoundTouch" \
    "-lopenal" \
    "-lzzip" \
    "-lz" \
    "-lpcap" \
    "-lGL" \
    "-lGLU" \
    "-llua5.4" \
    "-llua5.1" \
    "-lm"; do
    LINK_LIBS="$LINK_LIBS $libspec"
done

echo "Building hdrv..."
echo "  DeSmuME root: $DESMUME_ROOT"
echo "  SDL2 flags:   ${SDL_CFLAGS:-<pkg-config failed>}"
echo "  Output:       $OUTPUT"

g++ -std=gnu++17 -O2 \
    $DESMUME_INC \
    $SDL_CFLAGS \
    $GLIB_CFLAGS \
    $EXTRA_CFLAGS \
    tools/hdrv.cpp \
    "$LIB" \
    -o "$OUTPUT" \
    $LINK_LIBS

echo "Built $OUTPUT"
