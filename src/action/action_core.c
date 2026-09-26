/* Action system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 595-666 */

#include "nds_types.h"

extern ActionSysEntry g_actionsys[];
extern u32 g_actionsys_count;

extern void ActionTrigger_Setup(void*, void*, u32, u32);

#define MAX_ACTIONS 32

void Action_Init(void) {
    u32 i;
    g_actionsys_count = 0;
    for (i = 0; i < MAX_ACTIONS; i++) {
        g_actionsys[i].active = 0;
        g_actionsys[i].action_type = 0;
        g_actionsys[i].param = 0;
        g_actionsys[i].processed = 0;
    }
}

void Action_Teardown(void) {
    u32 i;
    for (i = 0; i < MAX_ACTIONS; i++) {
        g_actionsys[i].active = 0;
        g_actionsys[i].processed = 0;
    }
}

void Action_Process(void) {
    u32 i;
    for (i = 0; i < g_actionsys_count; i++) {
        if (g_actionsys[i].active && !g_actionsys[i].processed) {
            g_actionsys[i].processed = 1;
        }
    }
}
