/* Gameplay level boost gauge functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

void BoostGauge_Init(void) {
    g_boost_level = 0;
    g_boost_max = 100;
}

void BoostGauge_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y;
    if (g_boost_level < g_boost_max) {
        g_boost_level += param;
        if (g_boost_level > g_boost_max) g_boost_level = g_boost_max;
    }
}

s32 BoostGauge_Use(void) {
    if (g_boost_level <= 0) return 0;
    g_boost_level -= 2;
    if (g_boost_level < 0) g_boost_level = 0;
    return 1;
}

void BoostGauge_Update(void) {
    if (g_boostlaunch_active) {
        if (g_boost_level > 0) {
            g_boost_level -= 1;
            if (g_boost_level < 0) g_boost_level = 0;
        }
    }
}

void BoostGauge_Activate(void) {
    g_boostlaunch_active = 1;
}

void BoostGauge_Deactivate(void) {
    g_boostlaunch_active = 0;
}

u32 BoostGauge_Get(void) { return g_boost_level; }
u32 BoostGauge_GetMax(void) { return g_boost_max; }

s32 BoostGauge_IsActive(u32 index) {
    (void)index;
    return g_boostlaunch_active;
}

s32 BoostGauge_IsFull(void) {
    return g_boost_level >= g_boost_max;
}

s32 BoostGauge_IsEmpty(void) {
    return g_boost_level <= 0;
}

void BoostGauge_Set(u32 index, u32 value) {
    (void)index;
    g_boost_level = value;
    if (g_boost_level > g_boost_max) g_boost_level = g_boost_max;
}

void BoostGauge_SetMax(u32 index, u32 value) {
    (void)index;
    g_boost_max = value;
}

void BoostGauge_Reset(void) {
    g_boost_level = 0;
    g_boost_max = 100;
    g_boostlaunch_active = 0;
}
