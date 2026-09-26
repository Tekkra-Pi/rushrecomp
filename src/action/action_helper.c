/* Action helper functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* ActionHelper_Init @ 0x02014c00 (168 bytes)
 * Initializes action helper system.
 * Args: r0=action */
void ActionHelper_Init(void *action) {
    u32 *data = (u32*)action;
    
    /* Clear helper state */
    data[16] = 0;  /* +0x40: helper flags */
    data[17] = 0;  /* +0x44: helper data */
}

/* ActionHelper_SetFlag @ 0x02014ca8 (232 bytes)
 * Sets action helper flag.
 * Args: r0=action, r1=flag */
void ActionHelper_SetFlag(void *action, u32 flag) {
    u32 *data = (u32*)action;
    data[16] |= flag;
}

/* ActionHelper_ClearFlag @ 0x02014d90 (296 bytes)
 * Clears action helper flag.
 * Args: r0=action, r1=flag */
void ActionHelper_ClearFlag(void *action, u32 flag) {
    u32 *data = (u32*)action;
    data[16] &= ~flag;
}

/* ActionHelper_CheckFlag @ 0x02014eb8
 * Returns whether helper flag is set.
 * Args: r0=action, r1=flag
 * Returns: 1 if set, 0 otherwise */
s32 ActionHelper_CheckFlag(void *action, u32 flag) {
    u32 *data = (u32*)action;
    return (data[16] & flag) ? 1 : 0;
}

/* ActionHelper_SetData @ 0x02014f00
 * Sets action helper data.
 * Args: r0=action, r1=data */
void ActionHelper_SetData(void *action, u32 helper_data) {
    u32 *data = (u32*)action;
    data[17] = helper_data;
}

/* ActionHelper_GetData @ 0x02014f40
 * Returns action helper data.
 * Args: r0=action
 * Returns: helper data */
u32 ActionHelper_GetData(void *action) {
    u32 *data = (u32*)action;
    return data[17];
}

/* ActionHelper_Set @ 0x02014f80
 * Sets action helper.
 * Args: r0=action, r1=helper_id */
void ActionHelper_Set(void *action, u32 helper_id) {
    u32 *data = (u32*)action;
    data[16] = helper_id;
}

/* ActionHelper_Get @ 0x02014fc0
 * Returns action helper ID.
 * Args: r0=action
 * Returns: helper ID */
u32 ActionHelper_Get(void *action) {
    u32 *data = (u32*)action;
    return data[16];
}

/* ActionHelper_Clear @ 0x02015000
 * Clears action helper.
 * Args: r0=action */
void ActionHelper_Clear(void *action) {
    u32 *data = (u32*)action;
    data[16] = 0;
    data[17] = 0;
}

/* ActionHelper_Is @ 0x02015040
 * Checks if action matches helper.
 * Args: r0=action, r1=helper_id
 * Returns: 1 if matching, 0 otherwise */
s32 ActionHelper_Is(void *action, u32 helper_id) {
    u32 *data = (u32*)action;
    return data[16] == helper_id;
}
