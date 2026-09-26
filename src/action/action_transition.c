/* Action transition functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* ActionTransition_Init @ 0x02014400 (168 bytes)
 * Initializes action transition system.
 * Args: r0=action */
void ActionTransition_Init(void *action) {
    u32 *data = (u32*)action;
    
    /* Clear transition state */
    data[4] = 0;  /* +0x10: transition type */
    data[5] = 0;  /* +0x14: transition timer */
    data[6] = 0;  /* +0x18: transition duration */
}

/* ActionTransition_Start @ 0x020144a8 (232 bytes)
 * Starts action transition.
 * Args: r0=action, r1=type, r2=duration */
void ActionTransition_Start(void *action, u32 type, u32 duration) {
    u32 *data = (u32*)action;
    
    data[4] = type;       /* +0x10: type */
    data[5] = 0;          /* +0x14: timer */
    data[6] = duration;   /* +0x18: duration */
}

/* ActionTransition_Update @ 0x02014590 (296 bytes)
 * Updates action transition.
 * Args: r0=action
 * Returns: 1 if complete, 0 otherwise */
s32 ActionTransition_Update(void *action) {
    u32 *data = (u32*)action;
    
    if (data[6] == 0) return 1;  /* No transition */
    
    data[5]++;  /* Increment timer */
    
    if (data[5] >= data[6]) {
        /* Transition complete */
        data[4] = 0;
        data[5] = 0;
        data[6] = 0;
        return 1;
    }
    
    return 0;
}

/* ActionTransition_Get @ 0x020146b8
 * Returns transition type.
 * Args: r0=action
 * Returns: transition type */
u32 ActionTransition_Get(void *action) {
    u32 *data = (u32*)action;
    return data[4];
}

/* ActionTransition_GetProgress @ 0x020146f0
 * Returns transition progress.
 * Args: r0=action
 * Returns: progress (0-256) */
u32 ActionTransition_GetProgress(void *action) {
    u32 *data = (u32*)action;
    if (data[6] == 0) return 256;
    return (data[5] * 256) / data[6];
}

/* ActionTransition_IsActive @ 0x02014740
 * Returns whether transition is active.
 * Args: r0=action
 * Returns: 1 if active, 0 otherwise */
s32 ActionTransition_IsActive(void *action) {
    u32 *data = (u32*)action;
    return data[6] != 0;
}

/* ActionTransition_Cancel @ 0x02014780
 * Cancels action transition.
 * Args: r0=action */
void ActionTransition_Cancel(void *action) {
    u32 *data = (u32*)action;
    data[4] = 0;
    data[5] = 0;
    data[6] = 0;
}

/* ActionTransition_SetDuration @ 0x020147c0
 * Sets transition duration.
 * Args: r0=action, r1=duration */
void ActionTransition_SetDuration(void *action, u32 duration) {
    u32 *data = (u32*)action;
    data[6] = duration;
}

/* ActionTransition_GetDuration @ 0x02014800
 * Returns transition duration.
 * Args: r0=action
 * Returns: duration */
u32 ActionTransition_GetDuration(void *action) {
    u32 *data = (u32*)action;
    return data[6];
}
