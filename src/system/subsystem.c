/* Subsystem per-frame update functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "entity.h"

extern u32 g_system_flags;
extern void Overlay_TransitionStep(void);
extern void ObjListMgr_Update(void);
extern void ChunkLoader_Update(void);

void Subsystem_PerFrame(void) {
    if (!(g_system_flags & 1)) return;
    Overlay_TransitionStep();
    ObjListMgr_Update();
}

void PostEntityWalk(void) {
    extern void TouchState_ResetEnable(void);
    u32 state;
    TouchState_ResetEnable();
    state = 0;
    /* Process entity post-walk state */
    /* Walk entity list and handle state transitions */
    extern EntityContainer* Entity_GetHead(u32);
    EntityContainer* ent = Entity_GetHead(0);
    if (!ent) return;
    /* Check entity type for state transitions */
    while (ent) {
        u32 etype = ent->type;
        if (etype == 1) break;
        if (etype == 3) break;
        ent = ent->next;
    }
}
