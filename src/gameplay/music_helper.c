#include "nds_types.h"
void MusicHelper_Init(void) { g_current_music = 0; g_music_fading = 0; }
void MusicHelper_Play(u32 music_id) { g_current_music = music_id; }
void MusicHelper_Stop(void) { g_current_music = 0; }
void MusicHelper_FadeIn(void) { g_music_fading = 1; g_master_volume = 0; }
void MusicHelper_FadeOut(void) { g_music_fading = 2; }
void MusicHelper_Update(void) {
    if (g_music_fading == 1) { if (g_master_volume < 0x100) g_master_volume += 2; else g_music_fading = 0; }
    else if (g_music_fading == 2) { if (g_master_volume > 0) g_master_volume -= 2; else { g_music_fading = 0; g_current_music = 0; } }
}
u32 MusicHelper_GetCurrent(void) { return g_current_music; }
void MusicHelper_Reset(void) { g_current_music = 0; g_music_fading = 0; }
