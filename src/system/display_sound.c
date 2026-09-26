/* Display and sound initialization.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_master_volume;
extern u32 g_sfx_volume;
extern u32 g_music_volume;
extern u32 g_sfx_muted;

void Display_Init(void) {
    REG_DISPCNT = 0;
    REG_MASTER_BRIGHT = 0;
    REG_MASTER_BRIGHT_SUB = 0;
    REG_BG0CNT = 0;
    REG_BG1CNT = 0;
    REG_BG2CNT = 0;
    REG_BG3CNT = 0;
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = 0;
    REG_WIN0H = 0;
    REG_WIN1H = 0;
    REG_WIN0V = 0;
    REG_WIN1V = 0;
    REG_WININ = 0;
    REG_WINOUT = 0;
}

void Display_BacklightConfig(void) {
    /* BIT3=backlight power, BIT2=power LED */
    *(vu32*)0x04000304 = 0x8203;
}

void Sound_Init(void) {
    g_master_volume = 0x40;
    g_sfx_volume = 0x40;
    g_music_volume = 0x40;
    g_sfx_muted = 0;
    SOUND_MASTER_ENABLE = 0;
    SOUND_MASTER_ENABLE = SOUND_ENABLE_BIT;
    REG_SOUNDCNT = 0x000F;
    REG_MASTER_VOLUME = 0x80;
}
