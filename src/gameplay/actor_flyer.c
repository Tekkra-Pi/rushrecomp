/* Gameplay level actor flyer functions. */
#include "nds_types.h"


void ActorFlyer_Init(void) { g_actorflyer_count = 0; }

s32 ActorFlyer_Add(s32 x, s32 y, s32 end_x, s32 end_y, u32 speed) {
    if (g_actorflyer_count >= 16) return -1;
    u32 i = g_actorflyer_count;
    g_actorflyers[i].x = x; g_actorflyers[i].y = y;
    g_actorflyers[i].start_x = x; g_actorflyers[i].start_y = y;
    g_actorflyers[i].end_x = end_x; g_actorflyers[i].end_y = end_y;
    g_actorflyers[i].speed = speed; g_actorflyers[i].progress = 0;
    g_actorflyers[i].forward = 1; g_actorflyers[i].active = 1;
    g_actorflyer_count++; return i;
}

void ActorFlyer_Update(void) {
    u32 i;
    for (i = 0; i < g_actorflyer_count; i++) {
        if (!g_actorflyers[i].active) continue;
        if (g_actorflyers[i].forward) {
            g_actorflyers[i].progress += g_actorflyers[i].speed;
            if (g_actorflyers[i].progress >= 256) {
                g_actorflyers[i].progress = 256;
                g_actorflyers[i].forward = 0;
            }
        } else {
            if (g_actorflyers[i].progress <= g_actorflyers[i].speed) {
                g_actorflyers[i].progress = 0;
                g_actorflyers[i].forward = 1;
            } else {
                g_actorflyers[i].progress -= g_actorflyers[i].speed;
            }
        }
        g_actorflyers[i].x = g_actorflyers[i].start_x +
            (g_actorflyers[i].end_x - g_actorflyers[i].start_x) * g_actorflyers[i].progress / 256;
        g_actorflyers[i].y = g_actorflyers[i].start_y +
            (g_actorflyers[i].end_y - g_actorflyers[i].start_y) * g_actorflyers[i].progress / 256;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_actorflyers[i].x;
        s32 dy = py - g_actorflyers[i].y;
        if (dx >= -0x600 && dx <= 0x600 && dy >= -0x600 && dy <= 0x600 && !Player_IsInvincible()) {
            Player_Hurt();
        }
    }
}

void ActorFlyer_Remove(u32 i) { if (i < g_actorflyer_count) g_actorflyer_count--; }
u32 ActorFlyer_GetCount(void) { return g_actorflyer_count; }
void ActorFlyer_Clear(void) { g_actorflyer_count = 0; }
void ActorFlyer_Reset(void) { g_actorflyer_count = 0; }
