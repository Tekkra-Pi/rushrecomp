/* Dash panel functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Player_SetVX(s32 vx);
extern void Player_SetVY(s32 vy);

s32 DashPanel_CheckPlayer(void) {
    u32 i;
    for (i = 0; i < g_dashpanel_count; i++) {
        if (!g_dashpanels[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_dashpanels[i].x;
        s32 dy = py - g_dashpanels[i].y;
        if (dx >= -0x200 && dx <= g_dashpanels[i].w + 0x200 &&
            dy >= -0x200 && dy <= 0x200) {
            return i;
        }
    }
    return -1;
}

void DashPanel_Dash(void) {
    u32 i;
    for (i = 0; i < g_dashpanel_count; i++) {
        if (!g_dashpanels[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_dashpanels[i].x;
        s32 dy = py - g_dashpanels[i].y;
        if (dx >= -0x200 && dx <= g_dashpanels[i].w + 0x200 &&
            dy >= -0x200 && dy <= 0x200) {
            switch (g_dashpanels[i].dir) {
                case 0: /* right */
                    Player_SetVX(g_dashpanels[i].speed);
                    break;
                case 1: /* left */
                    Player_SetVX(-g_dashpanels[i].speed);
                    break;
                case 2: /* up */
                    Player_SetVY(-g_dashpanels[i].speed);
                    break;
                case 3: /* down */
                    Player_SetVY(g_dashpanels[i].speed);
                    break;
            }
            return;
        }
    }
}
