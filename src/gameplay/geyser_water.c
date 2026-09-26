/* Gameplay level geyser water functions. */
#include "nds_types.h"

void GeyserWater_Init(void) { }

void GeyserWater_Update(void) {
}

s32 GeyserWater_GetHeight(void) { return g_levelwater_level; }

void GeyserWater_SetHeight(s32 value) {
    g_levelwater_level = value;
}

void GeyserWater_Clear(void) { }
void GeyserWater_Reset(void) { g_levelwater_level = 0; }
