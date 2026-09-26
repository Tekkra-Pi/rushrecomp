/* Gameplay level item box functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Sound_Play(u32 sfx);

#define ITEMBOX_MAX 16

void ItemBox_Init(void) { }

s32 ItemBox_Add(s32 x, s32 y, u32 type) {
    u32 i = 0;
    g_itemboxes[i].x = x;
    g_itemboxes[i].y = y;
    g_itemboxes[i].type = type;
    g_itemboxes[i].active = 1;
    return i;
}

void ItemBox_Update(void) {
    u32 i;
    for (i = 0; i < ITEMBOX_MAX; i++) {
        if (!g_itemboxes[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_itemboxes[i].x;
        s32 dy = py - g_itemboxes[i].y;
        if (dx >= -0x300 && dx <= 0x300 && dy >= -0x300 && dy <= 0x300) {
            switch (g_itemboxes[i].type) {
                case 0: /* ring */
                    g_game_rings++;
                    break;
                case 1: /* speed shoes */
                    g_speedshoe_count++;
                    break;
                case 2: /* invincibility */
                    g_invinv_timer = 600;
                    break;
                case 3: /* shield */
                    g_shield_type = 1;
                    break;
            }
            g_itemboxes[i].active = 0;
            Sound_Play(0x05);
        }
    }
}

void ItemBox_Remove(u32 index) {
    if (index < ITEMBOX_MAX) g_itemboxes[index].active = 0;
}

u32 ItemBox_GetCount(void) { return ITEMBOX_MAX; }

void ItemBox_Clear(void) {
    u32 i;
    for (i = 0; i < ITEMBOX_MAX; i++) g_itemboxes[i].active = 0;
}

void ItemBox_Reset(void) {
    u32 i;
    for (i = 0; i < ITEMBOX_MAX; i++) g_itemboxes[i].active = 0;
}
