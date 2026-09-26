/* ─── PC Recomp: NDS Hardware Emulation Layer ───
 * Maps NDS memory-mapped I/O to emulated arrays so recompiled
 * code can run natively on PC. */

#include "nds_hw_stub.h"

/* Backing storage for NDS address spaces */
u32 nds_io_regs[PC_IO_SIZE / 4];      /* 0x04000000 - 0x04000FFF */
u16 nds_vram[PC_VRAM_SIZE / 2];       /* 0x06000000 - 0x060FFFFF */
u16 nds_oam[PC_OAM_SIZE / 2];         /* 0x07000000 - 0x070007FF */

/* NDS main RAM emulation (0x02000000 - 0x023FFFFF = 4MB) */
#define PC_MAIN_RAM_SIZE (4 * 1024 * 1024)
u8 nds_main_ram[PC_MAIN_RAM_SIZE];

/* Shared memory / NDS WRAM (0x027FF000 area) */
u8 nds_wram[0x1000];

/* Map a raw NDS address to emulated memory. Returns NULL if unmapped. */
void *nds_map_addr(u32 addr) {
    if (addr >= 0x04000000 && addr < 0x04000000 + PC_IO_SIZE)
        return &nds_io_regs[(addr - 0x04000000) >> 2];

    if (addr >= 0x06000000 && addr < 0x06000000 + PC_VRAM_SIZE)
        return &nds_vram[(addr - 0x06000000) >> 1];

    if (addr >= 0x07000000 && addr < 0x07000000 + PC_OAM_SIZE)
        return &nds_oam[(addr - 0x07000000) >> 1];

    if (addr >= 0x02000000 && addr < 0x02000000 + PC_MAIN_RAM_SIZE)
        return &nds_main_ram[addr - 0x02000000];

    if (addr >= 0x027FF000 && addr < 0x02800000)
        return &nds_wram[addr - 0x027FF000];

    return NULL;
}

/* Print emulation status */
void nds_hw_init(void) {
    memset(nds_io_regs, 0, sizeof(nds_io_regs));
    memset(nds_vram, 0, sizeof(nds_vram));
    memset(nds_oam, 0, sizeof(nds_oam));
    memset(nds_main_ram, 0, sizeof(nds_main_ram));
    memset(nds_wram, 0, sizeof(nds_wram));
    printf("[PC-Recomp] NDS hardware emulated: IO=%dKB VRAM=%dKB OAM=%dB RAM=%dMB\n",
           PC_IO_SIZE/1024, PC_VRAM_SIZE/1024, PC_OAM_SIZE, PC_MAIN_RAM_SIZE/(1024*1024));
}
