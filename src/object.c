/* ─── Object System (NDS-compatible) ─── */
#include "nds_types.h"
#include "entity.h"
#include "functions.h"

#define BM_OAM         (*(volatile u16*)(0x07000000))
#define DMAC_COUNT1    (*(volatile u32*)(0x02000004))

/* NDS VBlank handler - not implemented */
static void DMATransfer(u32 src, u32 dest, u16 words, u32 mode) {
    DMAC_COUNT1 = ((dest & 0x1FFFFFFF) << 16) | ((mode << 27) << 16) | words;
}

void OAMSetSprite(u32 id, s32 x, s32 y, u32 tile, u32 attr) {
    u16* oam = (u16*)(0x07000000 + (id * 8));
    oam[0] = (u16)x | (u16)((y + 16) << 8);
    oam[1] = (u16)tile | (u16)(0x1000 >> ((attr >> 24) & 7));  // Priority
    oam[2] = ((attr >> 16) & 0xFF) | 0x100;  // Attributes
    oam[3] = ((attr >> 24) & 0x3F) | 0x8000; // Size
}

void ObjectSetX(u32 id, s32 x) {
    u16* oam = (u16*)(0x07000000 + (id * 8));
    oam[0] = ((oam[0] & 0xFF00) | (u16)(x & 0xFF));
}

void ObjectSetY(u32 id, s32 y) {
    u16* oam = (u16*)(0x07000000 + (id * 8));
    oam[0] = ((oam[0] & 0x00FF) | (u16)(((y + 16) & 0xFF) << 8));
}

s32 ObjectGetX(u32 id) {
    u16* oam = (u16*)(0x07000000 + (id * 8));
    return (s32)(oam[0] & 0xFF);
}

s32 ObjectGetY(u32 id) {
    u16* oam = (u16*)(0x07000000 + (id * 8));
    return ((s32)(oam[0] >> 8) - 16);
}

void ObjectSetRot(u32 id, u32 rot) {
    u16* oam = (u16*)(0x07000000 + (id * 8));
    oam[5] = (u16)(rot & 0x3FF);
}

const s16 g_slope_direction_table[32] = {0};
