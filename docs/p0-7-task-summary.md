7. Use controlled tap/hold/move/release scripts to name touch bit `0x20`, bit `0x100`, fields `+0x0c/+0x0e`, `+0x14`, and `+0x16...` precisely.

Related work: P0-7-atmospheric.md describes controlled testing scripts at `scripts/p0-7-*.txt` and aims to identify field semantics via runtime telemetry. P0-7-1 onwards expects monotonic contact capture for release/older X/Y at `+0x0c`/`+0x0e`, coordinate-history format at `+0x16+`, and integration with the alternate path table `0x22b658c`.

Status: Scripts established; runtime results pending to close.