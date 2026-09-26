/* Chaos Emerald collection functions. */
#include "nds_types.h"

#define EMERALD_MAX 7
static u32 s_count;
static u32 s_emeralds[EMERALD_MAX];

void Emerald_Init(void) {
    u32 i; s_count = 0;
    for (i = 0; i < EMERALD_MAX; i++) s_emeralds[i] = 0;
}

s32 Emerald_Collect(void) {
    if (s_count >= EMERALD_MAX) return -1;
    s_emeralds[s_count] = 1; s_count++;
    SoundEffect_Play(0xd4, 0x60, 0x200);
    return s_count;
}

s32 Emerald_Check(void) { return (s32)s_count; }
u32 Emerald_GetCount(void) { return s_count; }
s32 Emerald_HasAll(void) { return s_count >= EMERALD_MAX ? 1 : 0; }
void Emerald_LoadSaveData(void) { }
void Emerald_SaveData(void) { }
