/* Sound effect functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_sfx_volume;
extern u32 g_sfx_muted;
extern u32 g_sfxhelper_last;

void SoundEffect_Init(void) {
    g_sfx_volume = 0x40;
    g_sfx_muted = 0;
    g_sfxhelper_last = 0;
}

void SoundEffect_Play(u32 index) {
    (void)index;
    if (g_sfx_muted) return;
    extern s32 SoundChannel_Alloc(void);
    SoundChannel_Alloc();
}

void SoundEffect_Stop(void) {
    extern void SoundChannel_Stop(void);
    SoundChannel_Stop();
}

void SoundEffect_StopAll(void) {
    extern void SoundChannel_Stop(void);
    SoundChannel_Stop();
}

void SoundEffect_SetVolume(u32 index, u32 value) {
    (void)index;
    g_sfx_volume = value;
}

void SoundEffect_Mute(void) {
    g_sfx_muted = 1;
}

s32 SoundEffect_IsPlaying(void) {
    extern SoundChannelEntry g_sound_channels[];
    u32 i;
    for (i = 0; i < 16; i++) {
        if (g_sound_channels[i].active) return 1;
    }
    return 0;
}
