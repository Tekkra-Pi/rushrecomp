/* Gameplay level gravity functions. */
#include "nds_types.h"

void Gravity_Init(void) {
    g_levelgravity_normal = 0x400;
}

void Gravity_Update(void) {
}

s32 Gravity_Get(void) { return g_levelgravity_normal; }

void Gravity_Set(s32 value) {
    g_levelgravity_normal = value;
}

void Gravity_Reset(void) {
    g_levelgravity_normal = 0x400;
}
