#!/usr/bin/env python3
"""Compare runtime memory captures to distinguish code, data, and BSS regions.

This tool analyzes memory dumps from different runtime phases (title, gameplay)
to identify which memory regions contain code, initialized data, or uninitialized
data (BSS).

Usage:
    compare-phases.py <dump1.bin> <dump2.bin> [--base1 ADDR1] [--base2 ADDR2]

Output:
    Comparison showing:
    - Regions that differ between phases (likely code/data)
    - Regions that are zero in both phases (likely BSS)
    - Regions that are identical (shared or unchanged)
"""

import argparse
import struct
import sys

def load_dump(path, base_addr=None):
    """Load a binary memory dump."""
    with open(path, "rb") as f:
        data = f.read()
    
    if base_addr is None:
        # Try to detect base from common NDS addresses
        if len(data) == 0x30000:  # 192 KiB = gameplay overlay
            base_addr = 0x02320000
        elif len(data) == 0x58380:  # Title overlay range
            base_addr = 0x022c5c80
        else:
            base_addr = 0x02000000
    
    return base_addr, data

def analyze_region(data, offset, size, threshold=0.1):
    """Analyze a memory region to determine its type."""
    if offset + size > len(data):
        size = len(data) - offset
    
    region = data[offset:offset + size]
    
    # Count zeros
    zero_count = sum(1 for b in region if b == 0)
    zero_ratio = zero_count / size if size > 0 else 0
    
    # Check for ARM/Thumb code patterns
    code_indicators = 0
    for i in range(0, min(size - 3, 1000), 4):
        word = struct.unpack_from("<I", region, i)[0]
        # Check for common ARM instructions
        if (word & 0x0F000000) == 0x0A000000:  # Branch
            code_indicators += 1
        if (word & 0x0E000000) == 0x08000000:  # Load/Store
            code_indicators += 1
    
    code_ratio = code_indicators / (min(size, 1000) // 4) if size >= 4 else 0
    
    # Determine type
    if zero_ratio > 0.95:
        return "BSS", zero_ratio, code_ratio
    elif code_ratio > 0.1:
        return "CODE", zero_ratio, code_ratio
    else:
        return "DATA", zero_ratio, code_ratio

def compare_dumps(dump1, dump2, base1, base2):
    """Compare two memory dumps and classify regions."""
    results = []
    
    # Align to common region
    common_start = max(base1, base2)
    common_end = min(base1 + len(dump1), base2 + len(dump2))
    
    if common_start >= common_end:
        return results
    
    # Analyze in 4KB chunks
    chunk_size = 0x1000
    for addr in range(common_start, common_end, chunk_size):
        offset1 = addr - base1
        offset2 = addr - base2
        
        region1 = dump1[offset1:offset1 + chunk_size]
        region2 = dump2[offset2:offset2 + chunk_size]
        
        # Pad shorter region
        if len(region1) < chunk_size:
            region1 = region1 + b'\x00' * (chunk_size - len(region1))
        if len(region2) < chunk_size:
            region2 = region2 + b'\x00' * (chunk_size - len(region2))
        
        # Compare
        identical = region1 == region2
        type1, zero1, code1 = analyze_region(dump1, offset1, chunk_size)
        type2, zero2, code2 = analyze_region(dump2, offset2, chunk_size)
        
        results.append({
            "addr": addr,
            "size": chunk_size,
            "identical": identical,
            "type1": type1,
            "type2": type2,
            "zero_ratio1": zero1,
            "zero_ratio2": zero2,
            "code_ratio1": code1,
            "code_ratio2": code2,
        })
    
    return results

def main():
    parser = argparse.ArgumentParser(description="Compare runtime memory phases")
    parser.add_argument("dump1", help="First memory dump file")
    parser.add_argument("dump2", help="Second memory dump file")
    parser.add_argument("--base1", type=lambda x: int(x, 0), help="Base address of dump1")
    parser.add_argument("--base2", type=lambda x: int(x, 0), help="Base address of dump2")
    args = parser.parse_args()
    
    # Load dumps
    base1, dump1 = load_dump(args.dump1, args.base1)
    base2, dump2 = load_dump(args.dump2, args.base2)
    
    print(f"Dump 1: {args.dump1}")
    print(f"  Base: {base1:#010x}")
    print(f"  Size: {len(dump1):#x} ({len(dump1)} bytes)")
    
    print(f"\nDump 2: {args.dump2}")
    print(f"  Base: {base2:#010x}")
    print(f"  Size: {len(dump2):#x} ({len(dump2)} bytes)")
    
    # Compare
    results = compare_dumps(dump1, dump2, base1, base2)
    
    # Summarize
    identical = sum(1 for r in results if r["identical"])
    different = sum(1 for r in results if not r["identical"])
    
    print(f"\nComparison Results:")
    print(f"  Total chunks: {len(results)}")
    print(f"  Identical: {identical}")
    print(f"  Different: {different}")
    
    # Classify regions
    code_regions = []
    data_regions = []
    bss_regions = []
    
    for r in results:
        if r["identical"]:
            # Check if it's BSS (mostly zeros)
            if r["zero_ratio1"] > 0.95:
                bss_regions.append(r)
            elif r["code_ratio1"] > 0.1:
                code_regions.append(r)
            else:
                data_regions.append(r)
        else:
            # Different between phases - likely code or data
            if r["code_ratio1"] > 0.1 or r["code_ratio2"] > 0.1:
                code_regions.append(r)
            else:
                data_regions.append(r)
    
    print(f"\nClassification:")
    print(f"  Code regions: {len(code_regions)}")
    print(f"  Data regions: {len(data_regions)}")
    print(f"  BSS regions: {len(bss_regions)}")
    
    # Print detailed results
    print(f"\nDetailed Analysis (showing non-identical regions):")
    for r in results:
        if not r["identical"]:
            print(f"  {r['addr']:#010x}: {r['type1']} vs {r['type2']} "
                  f"(zero: {r['zero_ratio1']:.2f}/{r['zero_ratio2']:.2f}, "
                  f"code: {r['code_ratio1']:.2f}/{r['code_ratio2']:.2f})")

if __name__ == "__main__":
    main()
