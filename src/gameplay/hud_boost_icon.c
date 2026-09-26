#include "nds_types.h"

void HudBoostIcon_Init(void) { g_hudboost_visible = 0; }
void HudBoostIcon_Update(void) { }
void HudBoostIcon_Draw(void) { }
void HudBoostIcon_Show(void) { g_hudboost_visible = 1; }
void HudBoostIcon_Hide(void) { g_hudboost_visible = 0; }
s32 HudBoostIcon_IsActive(void) { return g_hudboost_visible; }
void HudBoostIcon_Reset(void) { g_hudboost_visible = 0; }
