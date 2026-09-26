/* Gameplay level death plane functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Player_Kill(void);

void DeathPlane_Init(void) { }

s32 DeathPlane_Add(s32 x, s32 y, u32 param) {
    u32 i = g_exittrigger_count;
    g_deathplanes[i].x = x;
    g_deathplanes[i].y = y;
    g_deathplanes[i].w = 0x1000;
    g_deathplanes[i].h = 0x400;
    g_deathplanes[i].active = 1;
    return i;
}

void DeathPlane_Update(void) {
    u32 i;
    for (i = 0; i < g_exittrigger_count; i++) {
        if (!g_deathplanes[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        if (px >= g_deathplanes[i].x && px <= g_deathplanes[i].x + g_deathplanes[i].w &&
            py >= g_deathplanes[i].y && py <= g_deathplanes[i].y + g_deathplanes[i].h) {
            Player_Kill();
            return;
        }
    }
}

void DeathPlane_Remove(u32 index) {
    if (index < g_exittrigger_count) g_deathplanes[index].active = 0;
}

u32 DeathPlane_GetCount(void) { return g_exittrigger_count; }

void DeathPlane_Clear(void) { }
void DeathPlane_Reset(void) { }
