/* Sound/Music system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_sfx_volume;
extern u32 g_sfx_muted;
extern u32 g_master_volume;
extern u32 g_music_volume;
extern u32 g_current_music;
extern u32 g_current_zone_music;
extern u32 g_music_fading;

void Sound_ChannelConfig(void) {
    extern void SoundDevice_Init(void);
    SoundDevice_Init();
    SOUND_MASTER_ENABLE = SOUND_ENABLE_BIT;
    REG_SOUNDCNT = 0x000F;
    REG_MASTER_VOLUME = 0x80;
}

int Sound_LoadTrack(void) {
    return 0;
}

int Sound_PlayEffect(void) {
    extern s32 SoundChannel_Alloc(void);
    return SoundChannel_Alloc();
}
