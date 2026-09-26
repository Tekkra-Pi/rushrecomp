#include "nds_types.h"
s32 TitleCard_IsComplete(u32 index) { (void)index; return g_titlecard_state == 0 ? 1 : 0; }
u32 TitleCard_GetZone(void) { return g_titlecard_zone; }
u32 TitleCard_GetAct(void) { return g_titlecard_act; }
u32 TitleCard_GetAlpha(void) { return g_titlecard_alpha; }
void TitleCard_Stop(void) { g_titlecard_state = 0; }
