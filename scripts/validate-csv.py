#!/usr/bin/env python3
"""Validate hdrv CSV output against expected milestone values.

Usage:
    scripts/validate-csv.py <csv-file> [--milestones file]
    scripts/validate-csv.py <csv-file> --check-title
    scripts/validate-csv.py <csv-file> --check-gameplay

Milestone file format (one per line):
    frame,column_name,expected_hex_value

Built-in checks:
    --check-title:     frame ~1200 title-menu fields
    --check-gameplay:  frame ~1622 gameplay entry fields
"""
import argparse
import csv
import sys


# ── Built-in milestone definitions ──────────────────────────────────────────

TITLE_MILESTONES = [
    # (frame, column, expected_value, description, nonzero)
    # At title menu, touch should be enabled (flags bit0=1)
    (1200, "touchflags", 0x1, "touch enabled at title", False),
    # At title menu, no touch contact (0xffffffff sentinel)
    (1200, "touch0", 0xffffffff, "no contact X at title", False),
    (1200, "touch4", 0xffffffff, "no contact Y at title", False),
    # Entity head pointer should be valid (nonzero)
    (1200, "head", None, "entity head non-zero", True),
]

GAMEPLAY_MILESTONES = [
    # By frame 1622, gameplay should be entered (mode word = 3)
    (1622, "zmode", 0x3, "zone mode = gameplay", False),
    # At gameplay, touch should still be enabled
    (1622, "touchflags", 0x1, "touch enabled at gameplay", False),
]


def parse_hex(val):
    """Parse a hex string like '0x1234abcd' to int."""
    val = val.strip()
    if val.startswith("0x") or val.startswith("0X"):
        return int(val, 16)
    try:
        return int(val, 16)
    except ValueError:
        return int(val)


def load_milestones(path):
    """Load milestone definitions from a file."""
    milestones = []
    with open(path) as f:
        for lineno, line in enumerate(f, 1):
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = [p.strip() for p in line.split(",")]
            if len(parts) < 3:
                print(f"WARNING: {path}:{lineno}: expected frame,col,val, got {line}", file=sys.stderr)
                continue
            frame = int(parts[0])
            col = parts[1]
            val = parse_hex(parts[2]) if parts[2] != "nonzero" else None
            desc = parts[3] if len(parts) > 3 else f"{col} at frame {frame}"
            nonzero = parts[2] == "nonzero"
            milestones.append((frame, col, val, desc, nonzero))
    return milestones


def find_nearest_frame(rows, target, tolerance=60):
    """Find the row nearest to target frame within tolerance."""
    best = None
    best_dist = tolerance + 1
    for row in rows:
        fr = int(row["frame"])
        dist = abs(fr - target)
        if dist < best_dist:
            best_dist = dist
            best = row
    return best, best_dist


def validate_csv(csv_path, milestones, verbose=True):
    """Validate CSV against milestones. Returns number of failures."""
    with open(csv_path) as f:
        reader = csv.DictReader(f)
        rows = list(reader)

    if not rows:
        print("ERROR: CSV is empty", file=sys.stderr)
        return 1

    # Check column existence
    all_cols = set()
    for row in rows:
        all_cols.update(row.keys())

    failures = 0
    for frame, col, expected, desc, nonzero in milestones:
        if col not in all_cols:
            print(f"  SKIP  {desc}: column '{col}' not in CSV", file=sys.stderr)
            continue

        row, dist = find_nearest_frame(rows, frame)
        if row is None:
            print(f"  FAIL  {desc}: no frame near {frame}", file=sys.stderr)
            failures += 1
            continue

        actual = parse_hex(row[col])
        actual_frame = int(row["frame"])

        if nonzero:
            if actual == 0:
                print(f"  FAIL  {desc}: frame {actual_frame} {col}=0x{actual:x} (expected nonzero)", file=sys.stderr)
                failures += 1
            elif verbose:
                print(f"  OK    {desc}: frame {actual_frame} {col}=0x{actual:x} (nonzero)", file=sys.stderr)
        else:
            if actual != expected:
                print(f"  FAIL  {desc}: frame {actual_frame} {col}=0x{actual:x} (expected 0x{expected:x})", file=sys.stderr)
                failures += 1
            elif verbose:
                print(f"  OK    {desc}: frame {actual_frame} {col}=0x{actual:x}", file=sys.stderr)

    return failures


def main():
    parser = argparse.ArgumentParser(description="Validate hdrv CSV output")
    parser.add_argument("csv", help="CSV file to validate")
    parser.add_argument("--milestones", help="Milestone definitions file")
    parser.add_argument("--check-title", action="store_true",
                        help="Check title-menu milestones (~frame 1200)")
    parser.add_argument("--check-gameplay", action="store_true",
                        help="Check gameplay-entry milestones (~frame 1622)")
    parser.add_argument("-q", "--quiet", action="store_true",
                        help="Only print failures")
    args = parser.parse_args()

    milestones = []
    if args.milestones:
        milestones.extend(load_milestones(args.milestones))
    if args.check_title:
        milestones.extend(TITLE_MILESTONES)
    if args.check_gameplay:
        milestones.extend(GAMEPLAY_MILESTONES)

    if not milestones:
        parser.print_help()
        print("\nSpecify --check-title, --check-gameplay, or --milestones <file>")
        sys.exit(2)

    failures = validate_csv(args.csv, milestones, verbose=not args.quiet)
    if failures:
        print(f"\n{failures} milestone(s) FAILED", file=sys.stderr)
        sys.exit(1)
    else:
        print(f"\nAll {len(milestones)} milestone(s) passed", file=sys.stderr)
        sys.exit(0)


if __name__ == "__main__":
    main()
