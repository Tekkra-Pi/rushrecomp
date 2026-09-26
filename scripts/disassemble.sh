#!/bin/bash

# ARMIPS extraction framework for Sonic Rush (NDS) RE.
# Extracts an ARM9 function from the ROM, generates an ARMIPS reassembly harness,
# and verifies the reassembly is byte-exact against the ROM.

set -e
cd "$(dirname "$0")/.."

if [ $# -lt 1 ]; then
    echo "usage: $0 <ram_addr> [size]"
    echo "       size optional; auto-detected via radare2 if omitted"
    exit 1
fi

python3 scripts/extract.py "$@"
