#include "nds_types.h"
void LevelTrans_Init(void) { g_leveltrans_state = 0; }
void LevelTrans_Start(u32 dest_zone, u32 dest_x, u32 dest_y) {
    g_leveltrans_state = 1; g_levelwarp_dest_zone = dest_zone;
    g_levelwarp_dest_x = (s32)dest_x; g_levelwarp_dest_y = (s32)dest_y;
    g_transition_type = 0;
}
s32 LevelTrans_Update(void) {
    if (g_leveltrans_state == 0) return 0;
    g_transition_timer++;
    if (g_transition_timer >= g_transition_duration) { g_leveltrans_state = 0; return 1; }
    return 0;
}
u32 LevelTrans_GetState(void) { return g_leveltrans_state; }
void LevelTrans_Reset(void) { g_leveltrans_state = 0; g_transition_timer = 0; }
