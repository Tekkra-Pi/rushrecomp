#include "nds_types.h"

void HUDDraw_Init(void) { g_oamhelper_sprite_count = 0; }
s32 HUDDraw_AddSprite(void) {
    if (g_oamhelper_sprite_count >= 128) return -1;
    u32 i = g_oamhelper_sprite_count;
    g_hud_draw_list[i].sprite_id = 0; g_hud_draw_list[i].x = 0;
    g_hud_draw_list[i].y = 0; g_hud_draw_list[i].frame = 0; g_hud_draw_list[i].flags = 0;
    g_oamhelper_sprite_count++; return i;
}
void HUDDraw_RemoveSprite(void) { if (g_oamhelper_sprite_count > 0) g_oamhelper_sprite_count--; }
void HUDDraw_SetFrame(u32 index, u32 value) { if (index < g_oamhelper_sprite_count) g_hud_draw_list[index].frame = value; }
void HUDDraw_SetPosition(u32 index, u32 value) { if (index < g_oamhelper_sprite_count) g_hud_draw_list[index].x = (s32)value; }
void HUDDraw_SetFlags(u32 index, u32 value) { if (index < g_oamhelper_sprite_count) g_hud_draw_list[index].flags = value; }
void HUDDraw_Draw(void) { u32 i; for (i = 0; i < g_oamhelper_sprite_count; i++) OAM_AddSprite(g_hud_draw_list[i].x, g_hud_draw_list[i].y, g_hud_draw_list[i].sprite_id + g_hud_draw_list[i].frame, 0x2000); }
void HUDDraw_Clear(void) { g_oamhelper_sprite_count = 0; }
u32 HUDDraw_GetCount(void) { return g_oamhelper_sprite_count; }
