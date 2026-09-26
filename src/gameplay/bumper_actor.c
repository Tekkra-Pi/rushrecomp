/* Gameplay level bumper actor functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Player_SetVX(s32 vx);
extern void Player_SetVY(s32 vy);

#define BUMPERACTOR_MAX 16

static BouncePadEntry s_bumper_actors[BUMPERACTOR_MAX];

void BumperActor_Init(void) { g_bumperactor_count = 0; }

s32 BumperActor_Add(s32 x, s32 y, u32 param) {
    if (g_bumperactor_count >= BUMPERACTOR_MAX) return -1;
    u32 i = g_bumperactor_count;
    s_bumper_actors[i].x = x;
    s_bumper_actors[i].y = y;
    s_bumper_actors[i].active = 1;
    g_bumperactor_count++;
    return i;
}

void BumperActor_Update(void) {
}

void BumperActor_Trig(void) {
    u32 i;
    for (i = 0; i < g_bumperactor_count; i++) {
        if (!s_bumper_actors[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - s_bumper_actors[i].x;
        s32 dy = py - s_bumper_actors[i].y;
        if (dx >= -0x300 && dx <= 0x300 && dy >= -0x300 && dy <= 0x300) {
            Player_SetVX((dx > 0) ? 0x400 : -0x400);
            Player_SetVY((dy > 0) ? 0x400 : -0x400);
        }
    }
}

void BumperActor_Remove(u32 index) {
    if (index < g_bumperactor_count) s_bumper_actors[index].active = 0;
}

s32 BumperActor_GetX(u32 index) {
    if (index >= g_bumperactor_count) return 0;
    return s_bumper_actors[index].x;
}

s32 BumperActor_GetY(u32 index) {
    if (index >= g_bumperactor_count) return 0;
    return s_bumper_actors[index].y;
}

u32 BumperActor_GetCount(void) { return g_bumperactor_count; }

s32 BumperActor_IsActive(u32 index) {
    if (index >= g_bumperactor_count) return 0;
    return s_bumper_actors[index].active;
}

void BumperActor_Clear(void) { g_bumperactor_count = 0; }
void BumperActor_Reset(void) { g_bumperactor_count = 0; }
