/* Gameplay level lighting functions. */
#include "nds_types.h"


void Lighting_Init(void) { g_lighting_mode = 0; g_lighting_intensity = 16; }

void Lighting_SetMode(u32 mode) { g_lighting_mode = mode; }

void Lighting_SetIntensity(s32 intensity) {
    g_lighting_intensity = intensity;
    Display_SetMasterBright(intensity);
}

void Lighting_Update(void) {
    switch (g_lighting_mode) {
        case 0: /* normal */
            Display_SetMasterBright(0);
            break;
        case 1: /* darken */
            Display_SetMasterBright(-g_lighting_intensity);
            break;
        case 2: /* brighten */
            Display_SetMasterBright(g_lighting_intensity);
            break;
    }
}

u32 Lighting_GetMode(void) { return g_lighting_mode; }
s32 Lighting_GetIntensity(void) { return g_lighting_intensity; }
void Lighting_Reset(void) { g_lighting_mode = 0; g_lighting_intensity = 16; }
