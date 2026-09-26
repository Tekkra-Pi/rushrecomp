/* Gameplay level boost icon functions. */
#include "nds_types.h"

static u32 s_count;

void BoostIcon_Init(void) { s_count = 0; }

s32 BoostIcon_Add(s32 x, s32 y, u32 param) {
    if (s_count >= 4) return -1;
    g_hud_anims[s_count].x = x; g_hud_anims[s_count].y = y;
    g_hud_anims[s_count].sprite_id = param;
    g_hud_anims[s_count].active = 1;
    s_count++; return s_count - 1;
}

void BoostIcon_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (g_hud_anims[i].active) g_hud_anims[i].timer++;
    }
}

void BoostIcon_Draw(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_hud_anims[i].active) continue;
        OAM_AddSprite(g_hud_anims[i].x, g_hud_anims[i].y,
            g_hud_anims[i].sprite_id, 0x2000);
    }
}

void BoostIcon_Remove(u32 index) {
    if (index < s_count) g_hud_anims[index].active = 0;
}

u32 BoostIcon_GetCount(void) { return s_count; }
void BoostIcon_Clear(void) { s_count = 0; }
void BoostIcon_Reset(void) { s_count = 0; }
