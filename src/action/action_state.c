/* Action state functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern ActionSysEntry g_actionsys[];
extern u32 g_actionsys_count;
extern u32 g_actiontrig_count;

#define MAX_ACTIONS 32

void ActionState_Init(void) {
    u32 i;
    g_actionsys_count = 0;
    for (i = 0; i < MAX_ACTIONS; i++) {
        g_actionsys[i].active = 0;
        g_actionsys[i].action_type = 0;
    }
}

void ActionState_Set(u32 index, u32 value) {
    if (index < MAX_ACTIONS) {
        g_actionsys[index].action_type = value;
        g_actionsys[index].active = 1;
        g_actionsys[index].processed = 0;
    }
}

u32 ActionState_Get(void) {
    u32 i;
    for (i = 0; i < g_actionsys_count; i++) {
        if (g_actionsys[i].active) {
            return g_actionsys[i].action_type;
        }
    }
    return 0;
}

u32 ActionState_GetPrevious(void) {
    u32 i;
    for (i = 0; i < g_actionsys_count; i++) {
        if (g_actionsys[i].active) {
            return g_actionsys[i].param;
        }
    }
    return 0;
}

u32 ActionState_GetTimer(void) {
    return g_actiontrig_count;
}

void ActionState_Update(void) {
    u32 i;
    for (i = 0; i < g_actionsys_count; i++) {
        if (g_actionsys[i].active) {
            g_actionsys[i].processed = 0;
        }
    }
}

s32 ActionState_Is(void) {
    return g_actionsys_count > 0 ? 1 : 0;
}

void ActionState_FireCallback(void) {
    u32 i;
    for (i = 0; i < g_actionsys_count; i++) {
        if (g_actionsys[i].active && g_actionsys[i].processed) {
            g_actionsys[i].processed = 0;
        }
    }
}

void ActionState_Reset(void) {
    ActionState_Init();
}
