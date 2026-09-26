/* Gameplay level bounce pad functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Player_SetVY(s32 vy);

#define BOUNCEPAD_MAX 16

void BouncePad_Init(void) { g_bouncyfloor_count = 0; }

s32 BouncePad_Add(s32 x, s32 y, u32 param) {
    if (g_bouncyfloor_count >= BOUNCEPAD_MAX) return -1;
    u32 i = g_bouncyfloor_count;
    g_bouncepads[i].x = x;
    g_bouncepads[i].y = y;
    g_bouncepads[i].active = 1;
    g_bouncyfloor_count++;
    return i;
}

void BouncePad_Update(void) {
    u32 i;
    for (i = 0; i < g_bouncyfloor_count; i++) {
        if (!g_bouncepads[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_bouncepads[i].x;
        s32 dy = py - g_bouncepads[i].y;
        if (dx >= -0x300 && dx <= 0x300 && dy >= -0x100 && dy <= 0x100) {
            Player_SetVY(-0x600);
        }
    }
}

void BouncePad_Remove(u32 index) {
    if (index < g_bouncyfloor_count) g_bouncepads[index].active = 0;
}

u32 BouncePad_GetCount(void) { return g_bouncyfloor_count; }

void BouncePad_Clear(void) { g_bouncyfloor_count = 0; }
void BouncePad_Reset(void) { g_bouncyfloor_count = 0; }
