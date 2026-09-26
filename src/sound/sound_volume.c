/* Sound volume functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_master_volume;
extern u32 g_music_volume;
extern u32 g_sfx_volume;

void SoundVolume_Init(void) {
    g_master_volume = 0x40;
    g_music_volume = 0x40;
    g_sfx_volume = 0x40;
}

void SoundVolume_SetMaster(u32 index, u32 value) {
    (void)index;
    g_master_volume = value & 0x7F;
}

void SoundVolume_SetMusic(u32 index, u32 value) {
    (void)index;
    g_music_volume = value & 0x7F;
}

void SoundVolume_SetSfx(u32 index, u32 value) {
    (void)index;
    g_sfx_volume = value & 0x7F;
}

u32 SoundVolume_GetMaster(void) {
    return g_master_volume;
}

u32 SoundVolume_GetMusic(void) {
    return g_music_volume;
}

u32 SoundVolume_GetSfx(void) {
    return g_sfx_volume;
}

u32 SoundVolume_Apply(void) {
    REG_MASTER_VOLUME = (u16)(g_master_volume | 0x80);
    return g_master_volume;
}
