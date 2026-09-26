#include "nds_types.h"
void GrindInd_Init(void) { g_grindind_active = 0; }
void GrindInd_Update(void) { if (!g_grindind_active) return; s32 px, py; Player_GetPosition(&px, &py); g_grindind_x = px; g_grindind_y = py - 0x200; }
void GrindInd_Show(void) { g_grindind_active = 1; }
void GrindInd_Hide(void) { g_grindind_active = 0; }
s32 GrindInd_IsActive(void) { return g_grindind_active; }
void GrindInd_Reset(void) { g_grindind_active = 0; }
