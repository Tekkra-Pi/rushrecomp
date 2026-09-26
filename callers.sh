#!/bin/bash
# Count cross-references to each function in the ROM.
# Usage: ./callers.sh
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
source "${SCRIPT_DIR}/config.sh"

cd "$(dirname "$SONIC_RUSH_ROM")"
r2 -q -c "aaa; e scr.color=0; axt @@fcn.*" "$SONIC_RUSH_ROM" | grep -E 'fcn\.([0-9a-f]{8})' | grep -v '^$' | python3 -c "
import sys
from collections import Counter
calls = Counter()
for line in sys.stdin:
    parts = line.split()
    for p in parts:
        if p.startswith('fcn.') and len(p) > 8:
            target = p[4:12]
            calls[target] += 1
for t, n in calls.most_common():
    print(f'{n:5d}  fcn.0x{t}')
"
