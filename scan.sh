#!/bin/bash
# Scan ROM for hardware functions using radare2.
# Usage: ./scan.sh
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
source "${SCRIPT_DIR}/config.sh"

cd "$(dirname "$SONIC_RUSH_ROM")"
r2 -q -c 'aaa; e scr.color=0; pdf @@fcn.*' "$SONIC_RUSH_ROM" | python3 -c "
import sys, re
func = ''
for line in sys.stdin:
    if line.startswith('┌'):
        m = re.match(r'.*fcn\.([0-9a-f]{8}).*', line)
        func = m.group(1) if m else ''
    elif '0x4000' in line and func:
        print(func)
" | sort -u > "${PROJECT_ROOT}/hw_funcs.txt"
echo "=== $(wc -l < "${PROJECT_ROOT}/hw_funcs.txt") hardware functions ==="
