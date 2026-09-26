#!/usr/bin/env python3
"""Verify that compiled function binaries match the original ROM extraction.

Usage:
    scripts/verify-match.py <compiled.bin> <reference.bin>
    scripts/verify-match.py --all build/ disassembly/

Exit code 0 if all match, 1 if any mismatch.
"""
import sys
import os


def compare_bins(compiled, reference, name="<unknown>"):
    """Compare two binary files byte-for-byte. Returns (ok, details)."""
    if len(compiled) != len(reference):
        return False, f"{name}: size mismatch ({len(compiled)} vs {len(reference)} bytes)"

    mismatches = []
    for i in range(len(compiled)):
        if compiled[i] != reference[i]:
            mismatches.append((i, compiled[i], reference[i]))

    if not mismatches:
        return True, f"{name}: MATCH ({len(compiled)} bytes)"

    details = f"{name}: {len(mismatches)} byte(s) differ"
    for offset, got, expected in mismatches[:20]:
        details += f"\n  offset 0x{offset:03x}: got 0x{got:02x}, expected 0x{expected:02x}"
    if len(mismatches) > 20:
        details += f"\n  ... and {len(mismatches) - 20} more"
    return False, details


def verify_pair(compiled_path, reference_path):
    """Verify a single compiled-vs-reference pair."""
    with open(compiled_path, "rb") as f:
        compiled = f.read()
    with open(reference_path, "rb") as f:
        reference = f.read()
    name = os.path.basename(reference_path).replace(".bin", "")
    return compare_bins(compiled, reference, name)


def verify_all(build_dir, ref_dir):
    """Verify all .bin files in build_dir against ref_dir."""
    ok_count = 0
    fail_count = 0

    for fname in sorted(os.listdir(ref_dir)):
        if not fname.endswith(".bin"):
            continue
        ref_path = os.path.join(ref_dir, fname)
        # Look for matching compiled file
        # Compiled files may be in build/<addr>.text.bin or build/<addr>.bin
        candidates = [
            os.path.join(build_dir, fname),
            os.path.join(build_dir, fname.replace(".bin", ".text.bin")),
        ]
        compiled_path = None
        for c in candidates:
            if os.path.exists(c):
                compiled_path = c
                break

        if compiled_path is None:
            print(f"  SKIP  {fname} (no compiled version)")
            continue

        ok, details = verify_pair(compiled_path, ref_path)
        if ok:
            print(f"  OK    {details}")
            ok_count += 1
        else:
            print(f"  FAIL  {details}")
            fail_count += 1

    return fail_count == 0


def main():
    if len(sys.argv) < 3:
        print("usage: verify-match.py <compiled.bin> <reference.bin>")
        print("       verify-match.py --all <build-dir> <ref-dir>")
        sys.exit(2)

    if sys.argv[1] == "--all":
        build_dir = sys.argv[2]
        ref_dir = sys.argv[3]
        ok = verify_all(build_dir, ref_dir)
    else:
        ok, details = verify_pair(sys.argv[1], sys.argv[2])
        print(details)
        # Dump hex diff on mismatch
        if not ok:
            with open(sys.argv[1], "rb") as f:
                compiled = f.read()
            with open(sys.argv[2], "rb") as f:
                reference = f.read()
            for i in range(min(len(compiled), len(reference))):
                if compiled[i] != reference[i]:
                    print(f"  0x{i:03x}: compiled=0x{compiled[i]:02x} ref=0x{reference[i]:02x}")

    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
