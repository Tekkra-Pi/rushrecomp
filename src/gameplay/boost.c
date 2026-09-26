/* Boost gauge functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static void Boost_DeactivateInternal(void);

void Boost_Init(void) {
    g_boost_level = 0;
    g_boost_max = 100;
    g_boostlaunch_active = 0;
    g_boostchain_count = 0;
    g_boostchain_timer = 0;
}

void Boost_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y;
    if (g_boost_level < g_boost_max) {
        g_boost_level += param;
        if (g_boost_level > g_boost_max) g_boost_level = g_boost_max;
    }
}

void Boost_Activate(void) {
    g_boostlaunch_active = 1;
    g_boostchain_count = 0;
    g_boostchain_timer = 0;
}

void Boost_Update(void) {
    if (g_boostlaunch_active) {
        g_boostchain_timer++;
        if (g_boost_level > 0) {
            g_boost_level -= 1;
            if (g_boost_level < 0) g_boost_level = 0;
        }
        if (g_boost_level <= 0) {
            Boost_DeactivateInternal();
        }
    }
}

static void Boost_DeactivateInternal(void) {
    g_boostlaunch_active = 0;
}

void Boost_Deactivate(void) {
    Boost_DeactivateInternal();
}

s32 Boost_IsActive(u32 index) {
    (void)index;
    return g_boostlaunch_active;
}

u32 Boost_GetGauge(void) {
    return g_boost_level;
}
