#include "nds_types.h"
void PathFollow_Init(void) { g_pipeentry_count = 0; }
void PathFollow_Update(void) { }
s32 PathFollow_CheckPlayer(void) {
    u32 i;
    for (i = 0; i < g_pipeentry_count; i++) {
        if (!g_pipeentries[i].active) continue;
        s32 px, py; Player_GetPosition(&px, &py);
        s32 dx = px - g_pipeentries[i].x, dy = py - g_pipeentries[i].y;
        if (dx >= -0x200 && dx <= 0x200 && dy >= -0x200 && dy <= 0x200) return (s32)i;
    }
    return -1;
}
void PathFollow_MoveAlongPath(void) { }
void PathFollow_End(void) { }
