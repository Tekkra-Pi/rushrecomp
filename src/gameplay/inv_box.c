/* Gameplay level invincibility box functions. */
#include "nds_types.h"


void InvBox_Init(void) { g_invbox_count = 0; }

s32 InvBox_Add(s32 x, s32 y) {
    if (g_invbox_count >= 8) return -1;
    u32 i = g_invbox_count;
    g_invboxs[i].x = x; g_invboxs[i].y = y;
    g_invboxs[i].state = 0; g_invboxs[i].timer = 0;
    g_invboxs[i].active = 1;
    g_invbox_count++; return i;
}

void InvBox_Update(void) {
    u32 i;
    for (i = 0; i < g_invbox_count; i++) {
        if (!g_invboxs[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_invboxs[i].x;
        s32 dy = py - g_invboxs[i].y;
        if (dx >= -0x800 && dx <= 0x800 && dy >= -0x800 && dy <= 0x800 && Player_IsAttacking()) {
            if (g_invboxs[i].state == 0) {
                g_invboxs[i].state = 1;
                g_invboxs[i].timer = 600;
                Player_SetInvincible(1);
                SoundEffect_Play(0x1a, 0x40, 0x100);
            }
        }
        if (g_invboxs[i].state == 1) {
            g_invboxs[i].timer--;
            if (g_invboxs[i].timer <= 0) {
                g_invboxs[i].state = 0;
                Player_SetInvincible(0);
            }
        }
    }
}

void InvBox_Remove(u32 i) { if (i < g_invbox_count) g_invbox_count--; }
u32 InvBox_GetCount(void) { return g_invbox_count; }
void InvBox_Clear(void) { g_invbox_count = 0; }
void InvBox_Reset(void) { g_invbox_count = 0; }
