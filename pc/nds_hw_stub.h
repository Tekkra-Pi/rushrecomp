/* ─── PC Recomp: NDS Hardware Stub Layer ───
 * Replaces volatile memory-mapped NDS registers with emulated state. */
#ifndef NDS_HW_STUB_H
#define NDS_HW_STUB_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef volatile uint8_t  vu8;
typedef volatile uint16_t vu16;
typedef volatile uint32_t vu32;

/* NDS memory regions emulated as static arrays */
#define PC_VRAM_SIZE    (1024 * 1024)   /* 1MB VRAM */
#define PC_OAM_SIZE     (2 * 1024)      /* 2KB OAM */
#define PC_IO_SIZE      (4 * 1024)      /* 4KB I/O regs */

/* I/O register file — backs all 0x04xxxxxx accesses */
extern u32 nds_io_regs[PC_IO_SIZE / 4];
extern u16 nds_vram[PC_VRAM_SIZE / 2];
extern u16 nds_oam[PC_OAM_SIZE / 2];

/* Macro to read/write emulated registers at NDS addresses */
#define NDS_REG32(addr)  (nds_io_regs[((addr) - 0x04000000) >> 2])
#define NDS_REG16(addr)  (*(u16 *)&nds_io_regs[((addr) - 0x04000000) >> 2])

#endif
