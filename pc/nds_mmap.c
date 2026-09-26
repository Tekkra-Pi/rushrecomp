/* ─── PC Recomp: NDS Address Space Mapper ───
 * Uses mmap to map NDS hardware addresses (0x04xxxxxx, 0x06xxxxxx,
 * 0x07xxxxxx, 0x02xxxxxx) to emulated RAM on Linux, so recompiled
 * ARM code with hardcoded NDS addresses works unchanged on PC. */

#define _GNU_SOURCE
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void *map_fixed_region(uint64_t addr, size_t size, const char *name) {
    void *p = mmap((void *)addr, size,
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE,
                   -1, 0);
    if (p == MAP_FAILED) {
        /* Try without NOREPLACE for older kernels */
        p = mmap((void *)addr, size,
                 PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
                 -1, 0);
    }
    if (p == MAP_FAILED) {
        fprintf(stderr, "[PC-Recomp] FAILED to map %s at 0x%llx: %m\n",
                name, (unsigned long long)addr);
        return NULL;
    }
    memset(p, 0, size);
    return p;
}

int nds_mmap_init(void) {
    int ok = 1;

    /* NDS I/O registers: 0x04000000 - 0x04001000 (4KB) */
    if (!map_fixed_region(0x04000000, 0x2000, "IO_REGS")) ok = 0;

    /* NDS main RAM: 0x02000000 - 0x02400000 (4MB) */
    if (!map_fixed_region(0x02000000, 0x400000, "MAIN_RAM")) ok = 0;

    /* NDS shared WRAM: 0x027FF000 - 0x02800000 (4KB) */
    if (!map_fixed_region(0x027FF000, 0x1000, "SHARED_WRAM")) ok = 0;

    /* NDS VRAM: 0x06000000 - 0x06200000 (2MB) */
    if (!map_fixed_region(0x06000000, 0x200000, "VRAM")) ok = 0;

    /* NDS OAM: 0x07000000 - 0x07000800 (2KB) */
    if (!map_fixed_region(0x07000000, 0x1000, "OAM")) ok = 0;

    /* NDS palette RAM: 0x05000000 - 0x05000400 (1KB) */
    if (!map_fixed_region(0x05000000, 0x1000, "PALETTE")) ok = 0;

    /* NDS BIOS/exception vectors: 0x02000000 already mapped above */

    if (ok) {
        printf("[PC-Recomp] NDS address space mapped successfully\n");
        printf("  IO:    0x04000000 (8KB)\n");
        printf("  RAM:   0x02000000 (4MB)\n");
        printf("  VRAM:  0x06000000 (2MB)\n");
        printf("  OAM:   0x07000000 (4KB)\n");
        printf("  PAL:   0x05000000 (4KB)\n");
        printf("  WRAM:  0x027FF000 (4KB)\n");
    }
    return ok;
}
