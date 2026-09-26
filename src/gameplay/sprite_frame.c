#include "nds_types.h"
static u32 s_count;
void SpriteFrame_Init(void) { s_count = 0; }
s32 SpriteFrame_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y; (void)param;
    if (s_count >= 64) return -1;
    s_count++; return s_count - 1;
}
u32 SpriteFrame_GetTile(void) { return 0; }
u32 SpriteFrame_GetCount(void) { return s_count; }
void SpriteFrame_Clear(void) { s_count = 0; }
void SpriteFrame_Reset(void) { s_count = 0; }
