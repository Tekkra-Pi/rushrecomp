/* OAM/Sprite display functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_oam_count;
extern u32 g_oamhelper_sprite_count;

#define OAM_MAX_SPRITES 128

s32 OAM_AddSprite(void) {
    if (g_oam_count >= OAM_MAX_SPRITES) return -1;
    g_oam_count++;
    g_oamhelper_sprite_count++;
    return (s32)(g_oam_count - 1);
}

void OAM_RemoveSprite(void) {
    if (g_oam_count > 0) {
        g_oam_count--;
    }
}

void OAM_Update(void) {
    /* Copy OAM buffer to hardware OAM */
    extern u16 g_oam_buffer[];
    memcpy((void*)OAM, g_oam_buffer, 0x800);
    g_oam_count = 0;
    g_oamhelper_sprite_count = 0;
}
