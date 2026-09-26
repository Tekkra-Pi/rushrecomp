/* Gameplay level bumper pad functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Player_SetVX(s32 vx);
extern void Player_SetVY(s32 vy);

#define BUMPERPAD_MAX 16

static BouncePadEntry s_bumper_pads[BUMPERPAD_MAX];

void BumperPad_Init(void) { g_bumperpad_count = 0; }

s32 BumperPad_Add(s32 x, s32 y, u32 param) {
    if (g_bumperpad_count >= BUMPERPAD_MAX) return -1;
    u32 i = g_bumperpad_count;
    s_bumper_pads[i].x = x;
    s_bumper_pads[i].y = y;
    s_bumper_pads[i].active = 1;
    g_bumperpad_count++;
    return i;
}

void BumperPad_Update(void) {
}

void BumperPad_Trig(void) {
    u32 i;
    for (i = 0; i < g_bumperpad_count; i++) {
        if (!s_bumper_pads[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - s_bumper_pads[i].x;
        s32 dy = py - s_bumper_pads[i].y;
        if (dx >= -0x300 && dx <= 0x300 && dy >= -0x300 && dy <= 0x300) {
            Player_SetVX((dx > 0) ? 0x300 : -0x300);
            Player_SetVY((dy > 0) ? 0x300 : -0x300);
        }
    }
}

void BumperPad_Remove(u32 index) {
    if (index < g_bumperpad_count) s_bumper_pads[index].active = 0;
}

u32 BumperPad_GetCount(void) { return g_bumperpad_count; }

void BumperPad_Clear(void) { g_bumperpad_count = 0; }
void BumperPad_Reset(void) { g_bumperpad_count = 0; }
