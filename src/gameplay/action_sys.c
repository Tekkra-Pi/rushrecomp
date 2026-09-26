/* Gameplay level action system functions. */

#include "nds_types.h"

#define ACTIONSYS_MAX 32

static u32 s_actionsys_count;

void ActionSys_Init(void) { s_actionsys_count = 0; }

void ActionSys_Trigger(void) {
    u32 i;
    for (i = 0; i < s_actionsys_count; i++) {
        if (!g_actionsys[i].active) continue;
        g_actionsys[i].processed = 1;
    }
}

void ActionSys_Update(void) {
    u32 i;
    for (i = 0; i < s_actionsys_count; i++) {
        if (!g_actionsys[i].active) continue;
        if (g_actionsys[i].processed) {
            g_actionsys[i].active = 0;
        }
    }
}

void ActionSys_Clear(void) { s_actionsys_count = 0; }
void ActionSys_Reset(void) { s_actionsys_count = 0; }
