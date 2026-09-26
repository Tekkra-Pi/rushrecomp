#include "nds_types.h"
static u32 s_count;
void LevelFx_Init(void) { s_count = 0; }
s32 LevelFx_Add(s32 x, s32 y, u32 type, u32 param) {
    (void)x; (void)y; (void)type; (void)param;
    if (s_count >= 16) return -1;
    s_count++; return s_count - 1;
}
void LevelFx_Update(void) { }
void LevelFx_Remove(u32 index) { (void)index; }
u32 LevelFx_GetCount(void) { return s_count; }
void LevelFx_Clear(void) { s_count = 0; }
void LevelFx_Reset(void) { s_count = 0; }
