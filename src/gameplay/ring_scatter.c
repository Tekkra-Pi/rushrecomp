#include "nds_types.h"
static u32 s_count;
void RingScatter_Init(void) { s_count = 0; }
s32 RingScatter_Spawn(s32 x, s32 y, u32 param) {
    if (s_count >= 16) return -1;
    u32 i; u32 cnt = param < 16 ? param : 16;
    for (i = 0; i < cnt && s_count < 16; i++) {
        g_ringscat[s_count].x = x + ((i * 53) % 64) - 32;
        g_ringscat[s_count].y = y;
        g_ringscat[s_count].vx = ((i * 37) % 0x100) - 0x80;
        g_ringscat[s_count].vy = -0x200 - ((i * 29) % 0x100);
        g_ringscat[s_count].timer = 0; g_ringscat[s_count].active = 1;
        s_count++;
    }
    return s_count - 1;
}
void RingScatter_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_ringscat[i].active) continue;
        g_ringscat[i].x += g_ringscat[i].vx;
        g_ringscat[i].y += g_ringscat[i].vy;
        g_ringscat[i].vy += 0x20;
        g_ringscat[i].timer++;
        if (g_ringscat[i].timer >= 60) g_ringscat[i].active = 0;
    }
}
void RingScatter_Draw(void) { }
void RingScatter_Clear(void) { s_count = 0; }
void RingScatter_Reset(void) { s_count = 0; }
