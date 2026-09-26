/* Gameplay level object behavior helper functions. */
#include "nds_types.h"


void ObjBehavior_Init(void) { g_objbehavior_count = 0; }

s32 ObjBehavior_Add(u32 obj_id, u32 behavior) {
    if (g_objbehavior_count >= 64) return -1;
    u32 i = g_objbehavior_count;
    g_objbehaviors[i].obj_id = obj_id; g_objbehaviors[i].behavior = behavior;
    g_objbehaviors[i].timer = 0; g_objbehaviors[i].active = 1;
    g_objbehavior_count++; return i;
}

void ObjBehavior_Update(void) {
    u32 i;
    for (i = 0; i < g_objbehavior_count; i++) {
        if (!g_objbehaviors[i].active) continue;
        g_objbehaviors[i].timer++;
        switch (g_objbehaviors[i].behavior) {
            case 0: /* static */ break;
            case 1: /* patrol */
                Object_SetX(g_objbehaviors[i].obj_id,
                    Object_GetX(g_objbehaviors[i].obj_id) + (Math_Sin(g_objbehaviors[i].timer * 8) * 2));
                break;
            case 2: /* bob */
                Object_SetY(g_objbehaviors[i].obj_id,
                    Object_GetY(g_objbehaviors[i].obj_id) + (Math_Sin(g_objbehaviors[i].timer * 16) / 256));
                break;
            case 3: /* spin */
                Object_SetRot(g_objbehaviors[i].obj_id, g_objbehaviors[i].timer * 16);
                break;
        }
    }
}

void ObjBehavior_Remove(u32 i) { if (i < g_objbehavior_count) g_objbehavior_count--; }
u32 ObjBehavior_GetCount(void) { return g_objbehavior_count; }
void ObjBehavior_Clear(void) { g_objbehavior_count = 0; }
void ObjBehavior_Reset(void) { g_objbehavior_count = 0; }
