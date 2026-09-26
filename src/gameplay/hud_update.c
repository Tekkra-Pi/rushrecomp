#include "nds_types.h"

void HudUpdate_Init(void) { g_hud_update_flags = 0; g_hud_update_timer = 0; }
void HudUpdate_Update(void) { g_hud_update_timer++; if (g_hud_update_timer >= 4) { g_hud_update_timer = 0; g_hud_update_flags |= 1; } }
void HudUpdate_MarkDirty(u32 flags) { g_hud_update_flags |= flags; }
s32 HudUpdate_IsDirty(void) { return g_hud_update_flags != 0 ? 1 : 0; }
void HudUpdate_ClearFlags(void) { g_hud_update_flags = 0; }
void HudUpdate_Reset(void) { g_hud_update_flags = 0; g_hud_update_timer = 0; }
