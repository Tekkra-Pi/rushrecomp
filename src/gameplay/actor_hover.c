/* Gameplay level actor hover functions. */
#include "nds_types.h"


void ActorHover_Init(void) { g_actorhover_count = 0; }

s32 ActorHover_Add(s32 x, s32 y, s32 hover_range, u32 speed) {
    if (g_actorhover_count >= 16) return -1;
    u32 i = g_actorhover_count;
    g_actorhovers[i].x = x; g_actorhovers[i].y = y;
    g_actorhovers[i].origin_y = y; g_actorhovers[i].hover_range = hover_range;
    g_actorhovers[i].speed = speed; g_actorhovers[i].timer = 0;
    g_actorhovers[i].active = 1;
    g_actorhover_count++; return i;
}

void ActorHover_Update(void) {
    u32 i;
    for (i = 0; i < g_actorhover_count; i++) {
        if (!g_actorhovers[i].active) continue;
        g_actorhovers[i].timer++;
        g_actorhovers[i].y = g_actorhovers[i].origin_y +
            Math_Sin(g_actorhovers[i].timer * g_actorhovers[i].speed) * g_actorhovers[i].hover_range;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_actorhovers[i].x;
        s32 dy = py - g_actorhovers[i].y;
        if (dx >= -0x600 && dx <= 0x600 && dy >= -0x600 && dy <= 0x600 && !Player_IsInvincible()) {
            Player_Hurt();
        }
    }
}

void ActorHover_Remove(u32 i) { if (i < g_actorhover_count) g_actorhover_count--; }
u32 ActorHover_GetCount(void) { return g_actorhover_count; }
void ActorHover_Clear(void) { g_actorhover_count = 0; }
void ActorHover_Reset(void) { g_actorhover_count = 0; }
