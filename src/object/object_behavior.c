/* Object behavior functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_objbehavior_count;
extern ObjBehaviorEntry g_objbehaviors[];

#define MAX_OBJ_BEHAVIORS 64

void ObjectBehavior_Init(void) {
    u32 i;
    g_objbehavior_count = 0;
    for (i = 0; i < MAX_OBJ_BEHAVIORS; i++) {
        g_objbehaviors[i].active = 0;
    }
}

void ObjectBehavior_Update(void) {
    u32 i;
    for (i = 0; i < g_objbehavior_count; i++) {
        if (g_objbehaviors[i].active) {
            g_objbehaviors[i].timer++;
        }
    }
}

u32 ObjectBehavior_Get(void) {
    return g_objbehavior_count;
}

void ObjectBehavior_Set(u32 index, u32 value) {
    if (index < g_objbehavior_count) {
        g_objbehaviors[index].behavior = value;
    }
}

u32 ObjectBehavior_GetState(void) {
    u32 i;
    for (i = 0; i < g_objbehavior_count; i++) {
        if (g_objbehaviors[i].active) {
            return g_objbehaviors[i].state;
        }
    }
    return 0;
}

void ObjectBehavior_SetState(u32 index, u32 value) {
    if (index < g_objbehavior_count) {
        g_objbehaviors[index].state = value;
    }
}

u32 ObjectBehavior_GetTimer(void) {
    u32 i;
    for (i = 0; i < g_objbehavior_count; i++) {
        if (g_objbehaviors[i].active) {
            return g_objbehaviors[i].timer;
        }
    }
    return 0;
}

s32 ObjectBehavior_Is(void) {
    return g_objbehavior_count > 0 ? 1 : 0;
}
