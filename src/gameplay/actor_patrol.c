/* Gameplay level actor patrol functions. */
#include "nds_types.h"


void ActorPatrol_Init(void) { g_actorpatrol_count = 0; }

s32 ActorPatrol_Add(s32 x, s32 y, s32 range_x, s32 range_y, u32 speed) {
    if (g_actorpatrol_count >= 16) return -1;
    u32 i = g_actorpatrol_count;
    g_actorpatrols[i].x = x; g_actorpatrols[i].y = y;
    g_actorpatrols[i].origin_x = x; g_actorpatrols[i].origin_y = y;
    g_actorpatrols[i].range_x = range_x; g_actorpatrols[i].range_y = range_y;
    g_actorpatrols[i].speed = speed; g_actorpatrols[i].timer = 0;
    g_actorpatrols[i].active = 1;
    g_actorpatrol_count++; return i;
}

void ActorPatrol_Update(void) {
    u32 i;
    for (i = 0; i < g_actorpatrol_count; i++) {
        if (!g_actorpatrols[i].active) continue;
        g_actorpatrols[i].timer++;
        g_actorpatrols[i].x = g_actorpatrols[i].origin_x +
            Math_Sin(g_actorpatrols[i].timer * g_actorpatrols[i].speed) * g_actorpatrols[i].range_x;
        g_actorpatrols[i].y = g_actorpatrols[i].origin_y +
            Math_Cos(g_actorpatrols[i].timer * g_actorpatrols[i].speed) * g_actorpatrols[i].range_y;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_actorpatrols[i].x;
        s32 dy = py - g_actorpatrols[i].y;
        if (dx >= -0x600 && dx <= 0x600 && dy >= -0x600 && dy <= 0x600 && !Player_IsInvincible()) {
            Player_Hurt();
        }
    }
}

void ActorPatrol_Remove(u32 i) { if (i < g_actorpatrol_count) g_actorpatrol_count--; }
u32 ActorPatrol_GetCount(void) { return g_actorpatrol_count; }
void ActorPatrol_Clear(void) { g_actorpatrol_count = 0; }
void ActorPatrol_Reset(void) { g_actorpatrol_count = 0; }
