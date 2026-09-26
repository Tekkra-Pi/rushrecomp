/* Gameplay level fog functions. */
#include "nds_types.h"


void Fog_Init(void) { g_fog_active = 0; g_fog_color = 0; g_fog_density = 0; }

void Fog_Start(u32 color, u32 density) {
    g_fog_active = 1; g_fog_color = color; g_fog_density = density;
}

void Fog_Stop(void) { g_fog_active = 0; }

void Fog_Update(void) {
    if (!g_fog_active) return;
    Display_SetFog(g_fog_color, g_fog_density);
}

s32 Fog_IsActive(void) { return g_fog_active; }
void Fog_Reset(void) { g_fog_active = 0; g_fog_color = 0; g_fog_density = 0; }
