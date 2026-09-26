/* ─── Display/Video System (NDS-compatible) ─── */
#include "nds_types.h"
#include "functions.h"

#define DISP_CR         (*(volatile u32*)(0x04000000))
#define DISP_CR_COMMON  (*(volatile u32*)(0x04000004))
#define DISP_MX         (*(volatile u32*)(0x04000010))
#define DISP_MX_BG0     (*(volatile u32*)(0x04001004))
#define DISP_MX_BG0CNT   (*(volatile u32*)(0x04001000))
#define DIV_REG         (*(volatile u32*)(0x04000080))
#define LYCNT_REG       (*(volatile u32*)(0x04000004))
#define BG0CNT          (*(volatile u32*)(0x04000008))
#define KEYINPUT        (*(volatile u32*)(0x04000130))
#define FIFOA           (*(volatile u32*)(0x04000200))

void DisplayFadeFromBlack(void) {
    DIV_REG = 0;
}

void DisplayFillRect(s32 x, s32 y, s32 w, s32 h, u16 tile) {
    u16* bg = (u16*)0x06000000;
    for (u32 r = 0; r < h; r++) {
        u16* row = bg + ((y + r) * 256 + x);
        for (u32 c = 0; c < w; c++) {
            row[c] = tile;
        }
    }
}

void DisplayPrintFixed(s32 x, s32 y, const char *str) {
    if (!str) return;
    u16* vram = (u16*)(0x06000000 + (y * 256 + x));
    while (*str) {
        *vram++ = (u16)(*str++);
    }
}

void DisplaySetFog(u32 color, u32 density) {
    *(volatile u32*)(0x04001008) = (color << 8) | density;
}

void DisplaySetMasterBright(s32 intensity) {
    if (intensity < 0) intensity = 0;
    if (intensity > 16) intensity = 16;
    *(volatile u16*)(0x04001030) = (u16)(0x6B00 + intensity);
}
