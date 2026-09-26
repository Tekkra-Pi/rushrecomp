/* Gameplay level goal post functions. */
#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

void GoalPost_Init(void) { }

void GoalPost_Update(void) {
    u32 i;
    for (i = 0; i < g_signpost_count; i++) {
        if (!g_signposts[i].active) continue;
        g_signposts[i].timer++;
    }
}

void GoalPost_Spin(void) {
    u32 i;
    for (i = 0; i < g_signpost_count; i++) {
        if (!g_signposts[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_signposts[i].x;
        s32 dy = py - g_signposts[i].y;
        if (dx >= -0x400 && dx <= 0x400 && dy >= -0x400 && dy <= 0x400) {
            g_signposts[i].timer = 0;
        }
    }
}

u32 GoalPost_GetCount(void) { return g_signpost_count; }
void GoalPost_Clear(void) { g_signpost_count = 0; }
void GoalPost_Reset(void) { g_signpost_count = 0; }
