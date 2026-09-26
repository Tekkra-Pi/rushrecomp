#include "nds_types.h"
static u32 s_count;
void PalCycle_Init(void) { s_count = 0; g_palcycles = 0; }
s32 PalCycle_Add(u32 bg_id, u32 pal_bank, u32 speed, u32 len) {
    (void)bg_id; (void)pal_bank; (void)speed; (void)len;
    if (s_count >= 8) return -1;
    s_count++; g_palcycles = s_count; return s_count - 1;
}
void PalCycle_Update(void) { }
void PalCycle_Remove(u32 index) { (void)index; }
u32 PalCycle_GetCount(void) { return s_count; }
s32 PalCycle_IsActive(u32 index) { (void)index; return s_count > 0 ? 1 : 0; }
void PalCycle_Clear(void) { s_count = 0; g_palcycles = 0; }
void PalCycle_Reset(void) { PalCycle_Clear(); }
