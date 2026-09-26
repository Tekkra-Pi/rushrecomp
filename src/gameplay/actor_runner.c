/* Gameplay level actor runner functions. */
#include "nds_types.h"


void ActorRunner_Init(void) { g_actorrunner_count = 0; }

s32 ActorRunner_Add(s32 x, s32 y, u32 type, s32 speed, u32 dir) {
    if (g_actorrunner_count >= 16) return -1;
    u32 i = g_actorrunner_count;
    g_actorrunners[i].x = x; g_actorrunners[i].y = y;
    g_actorrunners[i].type = type; g_actorrunners[i].speed = speed;
    g_actorrunners[i].dir = dir; g_actorrunners[i].vx = 0; g_actorrunners[i].vy = 0;
    g_actorrunners[i].timer = 0; g_actorrunners[i].active = 1;
    g_actorrunner_count++; return i;
}

void ActorRunner_Update(void) {
    u32 i;
    for (i = 0; i < g_actorrunner_count; i++) {
        if (!g_actorrunners[i].active) continue;
        g_actorrunners[i].timer++;
        switch (g_actorrunners[i].dir) {
            case 0: g_actorrunners[i].vx = g_actorrunners[i].speed; break;
            case 1: g_actorrunners[i].vx = -g_actorrunners[i].speed; break;
            case 2: g_actorrunners[i].vy = g_actorrunners[i].speed; break;
            case 3: g_actorrunners[i].vy = -g_actorrunners[i].speed; break;
        }
        g_actorrunners[i].x += g_actorrunners[i].vx;
        g_actorrunners[i].y += g_actorrunners[i].vy;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_actorrunners[i].x;
        s32 dy = py - g_actorrunners[i].y;
        if (dx >= -0x800 && dx <= 0x800 && dy >= -0x800 && dy <= 0x800) {
            if (!Player_IsInvincible()) Player_Hurt();
        }
    }
}

void ActorRunner_Remove(u32 i) { if (i < g_actorrunner_count) g_actorrunner_count--; }
u32 ActorRunner_GetCount(void) { return g_actorrunner_count; }
void ActorRunner_Clear(void) { g_actorrunner_count = 0; }
void ActorRunner_Reset(void) { g_actorrunner_count = 0; }
