/* Object/Entity update and draw functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_objbehavior_count;
extern ObjBehaviorEntry g_objbehaviors[];

void ObjDraw(void) {
    extern void ObjListMgr_Draw(void);
    ObjListMgr_Draw();
}

void ObjMainUpdate(void) {
    extern void ObjListMgr_Update(void);
    ObjListMgr_Update();
}

void ObjBehavior(void) {
    u32 i;
    for (i = 0; i < g_objbehavior_count; i++) {
        if (g_objbehaviors[i].active) {
            g_objbehaviors[i].timer++;
        }
    }
}
