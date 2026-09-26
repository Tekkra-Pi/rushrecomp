/* Sound channel functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern SoundChannelEntry g_sound_channels[];
extern u32 g_sfx_volume;
extern u32 g_sfx_muted;

#define MAX_CHANNELS 16

void SoundChannel_Init(void) {
    u32 i;
    for (i = 0; i < MAX_CHANNELS; i++) {
        g_sound_channels[i].active = 0;
        g_sound_channels[i].id = i;
        g_sound_channels[i].type = 0;
        g_sound_channels[i].sound_id = 0;
        g_sound_channels[i].volume = 0x40;
        g_sound_channels[i].pitch = 0x400;
    }
}

s32 SoundChannel_Alloc(void) {
    u32 i;
    for (i = 0; i < MAX_CHANNELS; i++) {
        if (!g_sound_channels[i].active) {
            g_sound_channels[i].active = 1;
            return (s32)i;
        }
    }
    return -1;
}

void SoundChannel_Free(void) {
    u32 i;
    for (i = 0; i < MAX_CHANNELS; i++) {
        g_sound_channels[i].active = 0;
    }
}

void SoundChannel_Play(u32 index) {
    if (index < MAX_CHANNELS && g_sound_channels[index].active) {
        /* Start sound playback on channel */
    }
}

void SoundChannel_Stop(void) {
    u32 i;
    for (i = 0; i < MAX_CHANNELS; i++) {
        g_sound_channels[i].active = 0;
    }
}

void SoundChannel_SetVolume(u32 index, u32 value) {
    if (index < MAX_CHANNELS) {
        g_sound_channels[index].volume = value;
    }
}

void SoundChannel_SetPitch(u32 index, u32 value) {
    if (index < MAX_CHANNELS) {
        g_sound_channels[index].pitch = value;
    }
}
