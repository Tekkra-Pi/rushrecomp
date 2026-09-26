#include "nds_types.h"
static u32 s_state;
void GrindIndicator_Init(void) { s_state = 0; g_grindind_active = 0; }
void GrindIndicator_Show(void) { s_state = 1; g_grindind_active = 1; }
void GrindIndicator_Hide(void) { s_state = 0; g_grindind_active = 0; }
void GrindIndicator_Update(void) { if (s_state) g_grindind_y -= 0x80; }
s32 GrindIndicator_IsActive(void) { return s_state; }
void GrindIndicator_Reset(void) { s_state = 0; g_grindind_active = 0; }
