/* Stage event functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern EventEntry g_stage_events[];
extern u32 g_stageevent_count;

void StageEvent_Get(void) {
    /* Returns current stage event - implicit r0 return */
}

s32 StageEvent_GetById(void) {
    return 0;
}

s32 StageEvent_Check(void) {
    u32 i;
    for (i = 0; i < g_stageevent_count; i++) {
        if (g_stage_events[i].active) {
            return (s32)i;
        }
    }
    return -1;
}

void StageEvent_Fire(void) {
    u32 i;
    for (i = 0; i < g_stageevent_count; i++) {
        if (g_stage_events[i].active) {
            extern void EventSys_Trigger(u32, u32);
            EventSys_Trigger(g_stage_events[i].type, g_stage_events[i].param);
        }
    }
}
