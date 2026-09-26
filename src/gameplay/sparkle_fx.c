#include "nds_types.h"
static u32 s_count;
void SparkleFX_Init(void) { s_count = 0; }
s32 SparkleFX_Spawn(s32 x, s32 y, u32 param) {
    (void)x; (void)y; (void)param;
    if (s_count >= 16) return -1;
    s_count++; return s_count - 1;
}
void SparkleFX_Update(void) { }
void SparkleFX_Draw(void) { }
u32 SparkleFX_GetCount(void) { return s_count; }
void SparkleFX_Clear(void) { s_count = 0; }
void SparkleFX_Reset(void) { s_count = 0; }
