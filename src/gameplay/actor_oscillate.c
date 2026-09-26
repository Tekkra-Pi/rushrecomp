/* Gameplay level actor oscillate functions. */
#include "nds_types.h"


void ActorOscillate_Init(void) { g_actoroscillate_count = 0; }

s32 ActorOscillate_Add(s32 x, s32 y, u32 axis, s32 range, u32 speed) {
    if (g_actoroscillate_count >= 16) return -1;
    u32 i = g_actoroscillate_count;
    g_actoroscillates[i].x = x; g_actoroscillates[i].y = y;
    g_actoroscillates[i].origin_x = x; g_actoroscillates[i].origin_y = y;
    g_actoroscillates[i].axis = axis; g_actoroscillates[i].range = range;
    g_actoroscillates[i].speed = speed; g_actoroscillates[i].timer = 0;
    g_actoroscillates[i].active = 1;
    g_actoroscillate_count++; return i;
}

void ActorOscillate_Update(void) {
    u32 i;
    for (i = 0; i < g_actoroscillate_count; i++) {
        if (!g_actoroscillates[i].active) continue;
        g_actoroscillates[i].timer++;
        s32 offset = Math_Sin(g_actoroscillates[i].timer * g_actoroscillates[i].speed) * g_actoroscillates[i].range;
        if (g_actoroscillates[i].axis == 0) {
            g_actoroscillates[i].x = g_actoroscillates[i].origin_x + offset;
        } else {
            g_actoroscillates[i].y = g_actoroscillates[i].origin_y + offset;
        }
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_actoroscillates[i].x;
        s32 dy = py - g_actoroscillates[i].y;
        if (dx >= -0x600 && dx <= 0x600 && dy >= -0x600 && dy <= 0x600 && !Player_IsInvincible()) {
            Player_Hurt();
        }
    }
}

void ActorOscillate_Remove(u32 i) { if (i < g_actoroscillate_count) g_actoroscillate_count--; }
u32 ActorOscillate_GetCount(void) { return g_actoroscillate_count; }
void ActorOscillate_Clear(void) { g_actoroscillate_count = 0; }
void ActorOscillate_Reset(void) { g_actoroscillate_count = 0; }
