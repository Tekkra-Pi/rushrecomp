/* Gameplay level stage collision functions. */
#include "nds_types.h"


void StageCollision_Init(void) { g_stagecoll_active = 1; }

void StageCollision_Update(void) {
    if (!g_stagecoll_active) return;
    Player_CollisionCheck();
    ObjListMgr_Update();
}

void StageCollision_SetActive(s32 active) { g_stagecoll_active = active; }
s32 StageCollision_IsActive(void) { return g_stagecoll_active; }
void StageCollision_Reset(void) { g_stagecoll_active = 1; }
