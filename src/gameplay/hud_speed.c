#include "nds_types.h"

static u32 s_vis;
void HudSpeed_Init(void) { s_vis = 0; }
void HudSpeed_Update(void) { }
void HudSpeed_Draw(void) { }
void HudSpeed_Show(void) { s_vis = 1; }
void HudSpeed_Hide(void) { s_vis = 0; }
s32 HudSpeed_IsActive(void) { return s_vis; }
void HudSpeed_Reset(void) { s_vis = 0; }
