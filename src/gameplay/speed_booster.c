#include "nds_types.h"
static u32 s_count;
void SpeedBooster_Init(void) { s_count = 0; g_speedbooster_count = 0; }
s32 SpeedBooster_Add(s32 x, s32 y, u32 param) {
    (void)param;
    if (s_count >= 8) return -1;
    s_count++; g_speedbooster_count = s_count; return s_count - 1;
}
void SpeedBooster_Update(void) { }
void SpeedBooster_Remove(u32 index) { (void)index; }
u32 SpeedBooster_GetCount(void) { return s_count; }
void SpeedBooster_Clear(void) { s_count = 0; g_speedbooster_count = 0; }
void SpeedBooster_Reset(void) { SpeedBooster_Clear(); }
