#!/bin/bash
# Verify ROM integrity and extraction consistency
# Usage: verify-rom.sh [rom_path]

source "$(dirname "$0")/../config.sh"

ROM="${1:-$SONIC_RUSH_ROM}"

if [ ! -f "$ROM" ]; then
    echo "ERROR: ROM not found at $ROM"
    echo "Set SONIC_RUSH_ROM env var or pass path as argument"
    exit 1
fi

echo "ROM: $ROM"
echo "Size: $(wc -c < "$ROM") bytes"

# SHA-256 verification
SHA=$(sha256sum "$ROM" | cut -d' ' -f1)
echo "SHA-256: $SHA"

# Expected SHA-256 for Sonic Rush (USA) (En,Ja,Fr,De,Es,It).nds
EXPECTED="704d78ca48c6a3839e35e5c24c417588a73d8426693db0f0e0e4d73d6b1b20f4"
if [ "$SHA" = "$EXPECTED" ]; then
    echo "Status: VERIFIED"
else
    echo "Status: UNEXPECTED (expected $EXPECTED)"
    echo "This may be a different ROM version or dump"
fi

# Check extraction pairs exist
echo ""
echo "Extraction pairs:"
EXTRACTED=0
MATCHED=0
for bin in "$DISASSEMBLY_DIR"/*.bin; do
    [ -f "$bin" ] || continue
    EXTRACTED=$((EXTRACTED + 1))
    addr=$(basename "$bin" .bin)
    asm="${bin%.bin}.asm"
    if [ -f "$asm" ]; then
        MATCHED=$((MATCHED + 1))
    fi
done
echo "  Extracted: $EXTRACTED functions"
echo "  With .asm: $MATCHED"
