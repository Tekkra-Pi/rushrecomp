#include "nds_types.h"
void OamHelper_Init(void) { g_oam_count = 0; g_oamhelper_sprite_count = 0; }
void OamHelper_AddSprite(s32 x, s32 y, u32 tile, u32 attr) {
    (void)x; (void)y; (void)tile; (void)attr;
    if (g_oamhelper_sprite_count < 128) { g_oam_count++; g_oamhelper_sprite_count++; }
}
void OamHelper_RemoveSprite(void) {
    if (g_oamhelper_sprite_count > 0) { g_oamhelper_sprite_count--; if (g_oam_count > 0) g_oam_count--; }
}
void OamHelper_Update(void) { }
void OamHelper_Draw(void) { }
u32 OamHelper_GetCount(void) { return g_oamhelper_sprite_count; }
void OamHelper_Clear(void) { g_oam_count = 0; g_oamhelper_sprite_count = 0; }
void OamHelper_Reset(void) { OamHelper_Init(); }
