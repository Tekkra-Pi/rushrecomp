/* Sound music functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_current_music;
extern u32 g_current_zone_music;
extern u32 g_music_volume;
extern u32 g_music_fading;

void SoundMusic_Init(void) {
    g_current_music = 0;
    g_current_zone_music = 0;
    g_music_volume = 0x40;
    g_music_fading = 0;
}

void SoundMusic_Play(u32 index) {
    g_current_music = index;
    g_current_zone_music = index;
}

void SoundMusic_Stop(void) {
    g_current_music = 0;
}

void SoundMusic_Update(void) {
    if (g_music_fading) {
        if (g_music_volume > 0) {
            g_music_volume--;
        } else {
            g_music_fading = 0;
        }
    }
}

void SoundMusic_SetVolume(u32 index, u32 value) {
    (void)index;
    g_music_volume = value;
}

void SoundMusic_Pause(void) {
    /* Pause music playback */
}

void SoundMusic_Resume(void) {
    /* Resume music playback */
}

s32 SoundMusic_IsPlaying(void) {
    return g_current_music != 0 ? 1 : 0;
}
