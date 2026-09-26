#include "nds_types.h"
static u32 s_count;
void LivesIcon_Init(void) { s_count = 0; }
s32 LivesIcon_Add(s32 x, s32 y) {
    if (s_count >= 4) return -1;
    g_hud_anims[s_count].x = x; g_hud_anims[s_count].y = y;
    g_hud_anims[s_count].sprite_id = 0x200; g_hud_anims[s_count].active = 1;
    s_count++; return s_count - 1;
}
void LivesIcon_Update(void) { }
void LivesIcon_Draw(void) { }
void LivesIcon_Remove(u32 index) { if (index < s_count) g_hud_anims[index].active = 0; }
u32 LivesIcon_GetCount(void) { return s_count; }
void LivesIcon_Clear(void) { s_count = 0; }
void LivesIcon_Reset(void) { s_count = 0; }
