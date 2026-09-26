#!/bin/bash
# Rank functions by call count and size.
# Usage: ./rank.sh
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
source "${SCRIPT_DIR}/config.sh"

cd "$(dirname "$SONIC_RUSH_ROM")"

TMPDIR=$(mktemp -d)
trap 'rm -rf "$TMPDIR"' EXIT

r2 -q -c "aaa; e scr.color=0; axt @@fcn.*" "$SONIC_RUSH_ROM" | grep -oE 'fcn\.([0-9a-f]{8})' | cut -d. -f2 | sort | uniq -c | sort -rn > "${TMPDIR}/callcount.txt"
r2 -q -c "aaa; e scr.color=0; pdf @@fcn.*" "$SONIC_RUSH_ROM" | python3 -c "
import sys, re
func = ''
size = 0
for line in sys.stdin:
    m = re.match(r'┌ (\d+): fcn\.([0-9a-f]{8})', line)
    if m:
        func, size = m.group(2), int(m.group(1))
    elif '0x4000' in line and func:
        print(f'{func} {size}')
        func = ''
" | sort -u > "${TMPDIR}/hwsize.txt"
python3 -c "
counts = dict((a, int(b)) for b, a in (l.split() for l in open('${TMPDIR}/callcount.txt')))
for line in open('${TMPDIR}/hwsize.txt'):
    func, size = line.split()
    size, calls = int(size), counts.get(func, 0)
    print(f'{calls:5d} calls  {size:5d}B  fcn.0x{func}')
" | sort -rn | head -30
