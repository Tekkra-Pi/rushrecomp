#ifndef NDS_TYPES_H
#define NDS_TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef unsigned long long u64;
typedef long long s64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;

typedef unsigned int uint;
typedef unsigned char uchar;

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Include globals after base types are defined */
#include "gameplay_structs.h"
#include "globals.h"

#if defined(__arm__) || defined(__thumb__)
#define ARM9 __attribute__((target("arm")))
#define THUMB __attribute__((target("thumb")))
#else
#define ARM9
#define THUMB
#endif

#define TRUE 1
#define FALSE 0

/* NDS key bits */
#ifndef KEY_A
#define KEY_A      0x0001
#endif
#ifndef KEY_B
#define KEY_B      0x0002
#endif
#ifndef KEY_SELECT
#define KEY_SELECT 0x0004
#endif
#ifndef KEY_START
#define KEY_START  0x0008
#endif
#ifndef KEY_DRIGHT
#define KEY_DRIGHT 0x0010
#endif
#ifndef KEY_DLEFT
#define KEY_DLEFT  0x0020
#endif
#ifndef KEY_DUP
#define KEY_DUP    0x0040
#endif
#ifndef KEY_DDOWN
#define KEY_DDOWN  0x0080
#endif
#ifndef KEY_R
#define KEY_R      0x0100
#endif
#ifndef KEY_L
#define KEY_L      0x0200
#endif

/* Key aliases (some code uses these) */
#define KEY_LEFT   KEY_DLEFT
#define KEY_RIGHT  KEY_DRIGHT
#define KEY_UP     KEY_DUP
#define KEY_DOWN   KEY_DDOWN

/* Input subsystem aliases */
#define INPUT_A      KEY_A
#define INPUT_B      KEY_B
#define INPUT_SELECT KEY_SELECT
#define INPUT_START  KEY_START
#define INPUT_UP     KEY_DUP
#define INPUT_DOWN   KEY_DDOWN
#define INPUT_LEFT   KEY_DLEFT
#define INPUT_RIGHT  KEY_DRIGHT
#define INPUT_R      KEY_R
#define INPUT_L      KEY_L

/* NDS hardware registers */
#define REG_IPC_SYNC   (*(volatile u16 *)0x04000180)
#define REG_IME        (*(volatile u32 *)0x04000208)
#define REG_IE         (*(volatile u32 *)0x04000210)
#define REG_IF         (*(volatile u32 *)0x04000214)
#define REG_KEYINPUT   (*(volatile u16 *)0x04000130)

/* Display registers */
#ifndef REG_DISPCNT
#define REG_DISPCNT    (*(volatile u32 *)0x04000000)
#endif
#define REG_DISPSTAT   (*(volatile u16 *)0x04000004)
#define REG_VCOUNT     (*(volatile u16 *)0x04000006)
#define REG_BG0CNT     (*(volatile u16 *)0x04000008)
#define REG_BG1CNT     (*(volatile u16 *)0x0400000A)
#define REG_BG2CNT     (*(volatile u16 *)0x0400000C)
#define REG_BG3CNT     (*(volatile u16 *)0x0400000E)
#define REG_BG0HOFS    (*(volatile u16 *)0x04000010)
#define REG_BG0VOFS    (*(volatile u16 *)0x04000012)
#define REG_BG1HOFS    (*(volatile u16 *)0x04000014)
#define REG_BG1VOFS    (*(volatile u16 *)0x04000016)
#define REG_BG2HOFS    (*(volatile u16 *)0x04000018)
#define REG_BG2VOFS    (*(volatile u16 *)0x0400001A)
#define REG_BG3HOFS    (*(volatile u16 *)0x0400001C)
#define REG_BG3VOFS    (*(volatile u16 *)0x0400001E)
#define REG_MASTER_BRIGHT (*(volatile u16 *)0x0400006C)
#define REG_MASTER_BRIGHT_SUB (*(volatile u16 *)0x0400106C)

/* Window registers */
#define REG_WIN0H      (*(volatile u16 *)0x04000040)
#define REG_WIN1H      (*(volatile u16 *)0x04000042)
#define REG_WIN0V      (*(volatile u16 *)0x04000044)
#define REG_WIN1V      (*(volatile u16 *)0x04000046)
#define REG_WININ      (*(volatile u16 *)0x04000048)
#define REG_WINOUT     (*(volatile u16 *)0x0400004A)

/* VRAM control */
#define VRAM_A_CR      (*(volatile u32 *)0x04000240)
#define VRAM_B_CR      (*(volatile u32 *)0x04000241)
#define VRAM_C_CR      (*(volatile u32 *)0x04000242)
#define VRAM_D_CR      (*(volatile u32 *)0x04000243)
#define VRAM_E_CR      (*(volatile u32 *)0x04000244)
#define VRAM_F_CR      (*(volatile u32 *)0x04000245)
#define VRAM_G_CR      (*(volatile u32 *)0x04000246)

/* Sound */
#define REG_SOUNDCNT   (*(volatile u32 *)0x04000504)
#define REG_MASTER_VOLUME (*(volatile u16 *)0x04000500)

/* Sound enable */
#define SOUND_MASTER_ENABLE (*(volatile u32 *)0x04000504)
#define SOUND_ENABLE_BIT    (1 << 15)

/* OAM - Object Attribute Memory */
#define OAM           ((volatile u16 *)0x07000000)
#define OAM_SUB       ((volatile u16 *)0x07000400)

/* Palette RAM */
#define PALRAM        ((volatile u16 *)0x05000000)
#define PALRAM_SUB    ((volatile u16 *)0x05000400)

/* VRAM pointer */
#define VRAM          ((volatile u16 *)0x06800000)

/* IRQ */
#define IRQ_VBLANK    (1 << 0)
#define IRQ_HBLANK    (1 << 1)
#define IRQ_VCOUNT    (1 << 2)
#define IRQ_TIMER0    (1 << 3)
#define IRQ_TIMER1    (1 << 4)
#define IRQ_TIMER2    (1 << 5)
#define IRQ_TIMER3    (1 << 6)
#define IRQ_DMA0      (1 << 8)
#define IRQ_DMA1      (1 << 9)
#define IRQ_DMA2      (1 << 10)
#define IRQ_DMA3      (1 << 11)

/* DMA registers */
#define REG_DMA0SAD   (*(volatile u32 *)0x040000B0)
#define REG_DMA0DAD   (*(volatile u32 *)0x040000B4)
#define REG_DMA0CNT   (*(volatile u32 *)0x040000B8)
#define REG_DMA1SAD   (*(volatile u32 *)0x040000BC)
#define REG_DMA1DAD   (*(volatile u32 *)0x040000C0)
#define REG_DMA1CNT   (*(volatile u32 *)0x040000C4)
#define REG_DMA2SAD   (*(volatile u32 *)0x040000C8)
#define REG_DMA2DAD   (*(volatile u32 *)0x040000CC)
#define REG_DMA2CNT   (*(volatile u32 *)0x040000D0)
#define REG_DMA3SAD   (*(volatile u32 *)0x040000D4)
#define REG_DMA3DAD   (*(volatile u32 *)0x040000D8)
#define REG_DMA3CNT   (*(volatile u32 *)0x040000DC)

/* DMA flags */
#define DMA_ENABLE    (1 << 31)
#define DMA_START_NOW  (0 << 27)
#define DMA_REPEAT    (1 << 25)
#define DMA_32_BIT    (1 << 26)
#define DMA_16_BIT    (0 << 26)

/* Timer registers */
#define REG_TM0D      (*(volatile u16 *)0x04000100)
#define REG_TM0CNT    (*(volatile u32 *)0x04000102)
#define REG_TM1D      (*(volatile u16 *)0x04000104)
#define REG_TM1CNT    (*(volatile u32 *)0x04000106)
#define REG_TM2D      (*(volatile u16 *)0x04000108)
#define REG_TM2CNT    (*(volatile u32 *)0x0400010A)
#define REG_TM3D      (*(volatile u16 *)0x0400010C)
#define REG_TM3CNT    (*(volatile u32 *)0x0400010E)

/* Timer flags */
#define TIMER_ENABLE  (1 << 7)
#define TIMER_IRQ     (1 << 6)
#define TIMER_CASCADE (1 << 2)

/* MMU access stubs */
extern u8 _MMU_read08(int cpu, u32 addr);
extern u16 _MMU_read16(int cpu, u32 addr);
extern u32 _MMU_read32(int cpu, u32 addr);
extern void _MMU_write08(int cpu, u32 addr, u8 val);
extern void _MMU_write16(int cpu, u32 addr, u16 val);
extern void _MMU_write32(int cpu, u32 addr, u32 val);

#define ARMCPU_ARM9 0

/* String/memory helpers */
extern void *memset(void *s, int c, size_t n);
extern void *memcpy(void *dst, const void *src, size_t n);
extern int strcmp(const char *s1, const char *s2);
extern int strncmp(const char *s1, const char *s2, size_t n);
extern size_t strlen(const char *s);
extern char *strncpy(char *dst, const char *src, size_t n);
extern int sprintf(char *str, const char *fmt, ...);

/* Heap */
extern void *heap_alloc(u32 size, u32 tag);
extern void heap_free(void *ptr);

#endif /* NDS_TYPES_H */
