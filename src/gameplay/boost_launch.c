/* Gameplay boost launch functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static void BoostLaunch_StopInternal(void);
static u32 s_boostlaunch_active;
static u32 s_boostlaunch_timer;

void BoostLaunch_Init(void) {
    s_boostlaunch_active = 0;
    s_boostlaunch_timer = 0;
    g_boostlaunch_active = 0;
    g_boostlaunch_dir = 0;
}

void BoostLaunch_Start(void) {
    s_boostlaunch_active = 1;
    s_boostlaunch_timer = 0;
    g_boostlaunch_active = 1;
}

void BoostLaunch_Update(void) {
    if (!s_boostlaunch_active) return;
    s_boostlaunch_timer++;
    if (s_boostlaunch_timer >= 60) {
        BoostLaunch_StopInternal();
    }
}

void BoostLaunch_Draw(void) {
}

static void BoostLaunch_StopInternal(void) {
    s_boostlaunch_active = 0;
    g_boostlaunch_active = 0;
}

void BoostLaunch_Stop(void) {
    BoostLaunch_StopInternal();
}

s32 BoostLaunch_IsActive(u32 index) {
    (void)index;
    return s_boostlaunch_active;
}

u32 BoostLaunch_GetTimer(void) { return s_boostlaunch_timer; }

u32 BoostLaunch_GetDirection(void) { return g_boostlaunch_dir; }

void BoostLaunch_GetPosition(void) { }

void BoostLaunch_Reset(void) {
    s_boostlaunch_active = 0;
    s_boostlaunch_timer = 0;
    g_boostlaunch_active = 0;
    g_boostlaunch_dir = 0;
}
