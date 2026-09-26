#include "nds_types.h"
void GroundPound_Init(void) { g_gpound_timer = 0; }
void GroundPound_Start(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    g_gpound_x = px; g_gpound_y = py; g_gpound_timer = 16;
}
void GroundPound_Update(void) { if (g_gpound_timer > 0) { g_gpound_timer--; if (g_gpound_timer == 0) ShakeCamera(8, 4); } }
s32 GroundPound_IsActive(void) { return g_gpound_timer > 0 ? 1 : 0; }
void GroundPound_Reset(void) { g_gpound_timer = 0; }
