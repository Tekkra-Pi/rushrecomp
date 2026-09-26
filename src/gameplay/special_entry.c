#include "nds_types.h"
static u32 s_count;
void SpecialEntry_Init(void) { s_count = 0; g_specialentry_count = 0; }
s32 SpecialEntry_Add(s32 x, s32 y, u32 param) {
    (void)param;
    if (s_count >= 4) return -1;
    s_count++; g_specialentry_count = s_count; return s_count - 1;
}
void SpecialEntry_Update(void) { }
void SpecialEntry_Remove(u32 index) { (void)index; }
u32 SpecialEntry_GetCount(void) { return s_count; }
void SpecialEntry_Clear(void) { s_count = 0; g_specialentry_count = 0; }
void SpecialEntry_Reset(void) { SpecialEntry_Clear(); }
