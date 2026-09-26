/* Stage collision functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_stage_collision_width;
extern u32 g_stagecoll_active;
extern u32 g_layer_collision_matrix[];
extern u32 g_collision_debug;

void StageCollision_Load(void) {
    g_stagecoll_active = 1;
    g_stage_collision_width = 0;
    g_collision_debug = 0;
}

u8 StageCollision_GetTile(void) {
    return 0;
}

u8 StageCollision_GetSlope(void) {
    return 0;
}

s32 StageCollision_GetHeight(void) {
    return 0;
}

s32 StageCollision_Check(void) {
    return 0;
}

s32 StageCollision_CheckPoint(void) {
    return 0;
}

u32 StageCollision_GetWidth(void) {
    return g_stage_collision_width;
}

u32 StageCollision_GetHeightMap(void) {
    return g_layer_collision_matrix[0];
}
