/* Gameplay level actor stomper functions. */
#include "nds_types.h"


void ActorStomper_Init(void) { g_actorstomper_count = 0; }

s32 ActorStomper_Add(s32 x, s32 y, s32 range, u32 speed) {
    if (g_actorstomper_count >= 8) return -1;
    u32 i = g_actorstomper_count;
    g_actorstompers[i].x = x; g_actorstompers[i].y = y;
    g_actorstompers[i].origin_y = y;
    g_actorstompers[i].range = range; g_actorstompers[i].speed = speed;
    g_actorstompers[i].stomping = 0; g_actorstompers[i].timer = 0;
    g_actorstompers[i].active = 1;
    g_actorstomper_count++; return i;
}

void ActorStomper_Update(void) {
    u32 i;
    for (i = 0; i < g_actorstomper_count; i++) {
        if (!g_actorstompers[i].active) continue;
        g_actorstompers[i].timer++;
        if (!g_actorstompers[i].stomping) {
            if (g_actorstompers[i].timer % (g_actorstompers[i].speed * 60) == 0) {
                g_actorstompers[i].stomping = 1;
            }
        }
        if (g_actorstompers[i].stomping) {
            g_actorstompers[i].y += 0x100;
            if (g_actorstompers[i].y >= g_actorstompers[i].origin_y + g_actorstompers[i].range) {
                g_actorstompers[i].y = g_actorstompers[i].origin_y + g_actorstompers[i].range;
                g_actorstompers[i].stomping = 0;
            }
        } else {
            if (g_actorstompers[i].y > g_actorstompers[i].origin_y) {
                g_actorstompers[i].y -= 0x40;
            }
        }
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_actorstompers[i].x;
        s32 dy = py - g_actorstompers[i].y;
        if (dx >= -0x800 && dx <= 0x800 && dy >= -0x400 && dy <= 0x400 && !Player_IsInvincible()) {
            Player_Hurt();
        }
    }
}

void ActorStomper_Remove(u32 i) { if (i < g_actorstomper_count) g_actorstomper_count--; }
u32 ActorStomper_GetCount(void) { return g_actorstomper_count; }
void ActorStomper_Clear(void) { g_actorstomper_count = 0; }
void ActorStomper_Reset(void) { g_actorstomper_count = 0; }
