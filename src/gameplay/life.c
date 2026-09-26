#include "nds_types.h"
void Life_Init(void) { g_lives = 3; g_life_max = 99; }
void Life_Add(void) { if (g_lives < g_life_max) g_lives++; }
void Life_Remove(void) { if (g_lives > 0) g_lives--; }
u32 Life_Get(void) { return g_lives; }
u32 Life_GetMax(void) { return g_life_max; }
void Life_Set(u32 val) { g_lives = val; }
void Life_SetMax(u32 val) { g_life_max = val; }
void Life_Reset(void) { g_lives = 3; }
