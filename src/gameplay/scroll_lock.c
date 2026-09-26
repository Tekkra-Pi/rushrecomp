/* Gameplay level scroll lock functions. */
#include "nds_types.h"


void ScrollLock_Init(void) { g_scrolllock_count = 0; }

s32 ScrollLock_Add(s32 x, s32 y, s32 w, s32 h, u32 lock_type, s32 lock_val) {
    if (g_scrolllock_count >= 8) return -1;
    u32 i = g_scrolllock_count;
    g_scrolllocks[i].x = x; g_scrolllocks[i].y = y;
    g_scrolllocks[i].w = w; g_scrolllocks[i].h = h;
    g_scrolllocks[i].lock_type = lock_type; g_scrolllocks[i].lock_val = lock_val;
    g_scrolllocks[i].triggered = 0; g_scrolllocks[i].active = 1;
    g_scrolllock_count++; return i;
}

void ScrollLock_Update(void) {
    u32 i;
    for (i = 0; i < g_scrolllock_count; i++) {
        if (!g_scrolllocks[i].active || g_scrolllocks[i].triggered) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        if (px >= g_scrolllocks[i].x && px <= g_scrolllocks[i].x + g_scrolllocks[i].w &&
            py >= g_scrolllocks[i].y && py <= g_scrolllocks[i].y + g_scrolllocks[i].h) {
            g_scrolllocks[i].triggered = 1;
            switch (g_scrolllocks[i].lock_type) {
                case 0: /* lock scroll X */
                    CameraBounds_SetLockX(g_scrolllocks[i].lock_val);
                    break;
                case 1: /* lock scroll Y */
                    CameraBounds_SetLockY(g_scrolllocks[i].lock_val);
                    break;
            }
        }
    }
}

void ScrollLock_Remove(u32 i) { if (i < g_scrolllock_count) g_scrolllock_count--; }
u32 ScrollLock_GetCount(void) { return g_scrolllock_count; }
void ScrollLock_Clear(void) { g_scrolllock_count = 0; }
void ScrollLock_Reset(void) { g_scrolllock_count = 0; }
