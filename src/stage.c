/* ─── Stage Collision System ─── */
#include "nds_types.h"
#include "player.h"
#include "gameplay_structs.h"


u32 StageCollision_GetFloor(s32 x, s32 y) {
    if (!g_current_player) return 0;
    
    /* Basic floor check based on PhysicsPlayer state */
    s32 px = g_current_player->pos_x;
    s32 py = g_current_player->pos_y;
    
    if (y > py) return 0;  // Current position is below
    
    /* For now return positive if above current position */
    /* (approximate - would need tile data lookup for real implementation) */
    return (y < py) ? 1 : 0;
}

u32 StageCollision_GetCeiling(s32 x, s32 y) {
    if (!g_current_player) return 0;
    
    s32 py = g_current_player->pos_y;
    
    if (y < py) return 0;  // Current position is above
    
    return (y > py) ? 1 : 0;
}

u32 StageCollision_GetWallLeft(s32 x, s32 y) {
    if (!g_current_player) return 0;
    
    /* Check if x is left of player */
    s32 px = g_current_player->pos_x;
    
    if (x < px) return 1;  /* Hit left wall */
    return 0;
}

u32 StageCollision_GetWallRight(s32 x, s32 y) {
    if (!g_current_player) return 0;
    
    /* Check if x is right of player */
    s32 px = g_current_player->pos_x;
    
    if (x > px) return 1;  /* Hit right wall */
    return 0;
}

void StageChunk_Load(u32 cx, u32 cy) {
    (void)cx; (void)cy;
    /* Would load chunk from ROM and populate collision tables */
    /* For now - no-op */
}
