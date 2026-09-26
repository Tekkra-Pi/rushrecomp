#include "nds_types.h"

extern void Player_GetPosition(s32 *x, s32 *y);

void TimePost_Update(void) {
    u32 i;
    for (i = 0; i < g_signpost_count; i++) {
        if (!g_signposts[i].active) continue;
        g_signposts[i].timer++;
    }
}
s32 TimePost_CheckPlayer(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    u32 i;
    for (i = 0; i < g_signpost_count; i++) {
        if (!g_signposts[i].active) continue;
        s32 dx = px - g_signposts[i].x, dy = py - g_signposts[i].y;
        if (dx >= -0x200 && dx <= 0x200 && dy >= -0x200 && dy <= 0x200) return (s32)i;
    }
    return -1;
}
void TimePost_Init(void) { g_signpost_count = 0; }
s32 Signpost_CheckPlayer(void) { return TimePost_CheckPlayer(); }
s32 Signpost_CheckGoal(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    u32 i;
    for (i = 0; i < g_signpost_count; i++) {
        if (!g_signposts[i].active) continue;
        s32 dx = px - g_signposts[i].x, dy = py - g_signposts[i].y;
        if (dx >= -0x200 && dx <= 0x200 && dy >= -0x200 && dy <= 0x200) return (s32)i;
    }
    return 0;
}
