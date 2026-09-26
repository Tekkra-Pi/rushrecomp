#include "nds_types.h"
static u32 s_count;
s32 SpriteAnim_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y; (void)param;
    if (s_count >= 32) return -1;
    s_count++; return s_count - 1;
}
void SpriteAnim_SetAnim(u32 index, u32 value) { (void)index; (void)value; }
void SpriteAnim_Remove(u32 index) { (void)index; }
u32 SpriteAnim_GetCount(void) { return s_count; }
void SpriteAnim_Clear(void) { s_count = 0; }
void SpriteAnim_Reset(void) { s_count = 0; }
