#include "nds_types.h"
static u32 s_count;
void MusicTrigger_Init(void) { s_count = 0; }
s32 MusicTrigger_Add(s32 x, s32 y, u32 music_id) {
    (void)x; (void)y; (void)music_id;
    if (s_count >= 8) return -1;
    s_count++; return s_count - 1;
}
void MusicTrigger_Update(void) { }
void MusicTrigger_Remove(u32 index) { (void)index; }
u32 MusicTrigger_GetCount(void) { return s_count; }
void MusicTrigger_Clear(void) { s_count = 0; }
void MusicTrigger_Reset(void) { s_count = 0; }
