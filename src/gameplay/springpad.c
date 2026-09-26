/* Spring pad functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Player_SetY(s32 y);
extern void Player_SetVY(s32 vy);

#define SPRINGPAD_MAX 16

s32 SpringPad_CheckPlayer(void) {
    u32 i;
    for (i = 0; i < g_springpad_count; i++) {
        if (!g_springpads[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_springpads[i].x;
        s32 dy = py - g_springpads[i].y;
        if (dx >= -0x200 && dx <= 0x200 && dy >= -0x200 && dy <= 0x200) {
            return i;
        }
    }
    return -1;
}

void SpringPad_Spring(void) {
    u32 i;
    for (i = 0; i < g_springpad_count; i++) {
        if (!g_springpads[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_springpads[i].x;
        s32 dy = py - g_springpads[i].y;
        if (dx >= -0x200 && dx <= 0x200 && dy >= -0x200 && dy <= 0x200) {
            g_springpads[i].anim = 1;
            switch (g_springpads[i].dir) {
                case 0: /* up */
                    Player_SetY(g_springpads[i].y - 0x400);
                    Player_SetVY(-g_springpads[i].force);
                    break;
                case 1: /* down */
                    Player_SetY(g_springpads[i].y + 0x400);
                    Player_SetVY(g_springpads[i].force);
                    break;
                case 2: /* left */
                    break;
                case 3: /* right */
                    break;
            }
            return;
        }
    }
}
