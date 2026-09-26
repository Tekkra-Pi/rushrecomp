#include "nds_types.h"
static u32 s_count;
void ItemSpawn_Init(void) { s_count = 0; }
s32 ItemSpawn_Add(s32 x, s32 y, u32 type) {
    if (s_count >= 32) return -1;
    g_itemspawns[s_count].x = x; g_itemspawns[s_count].y = y;
    g_itemspawns[s_count].type = type; g_itemspawns[s_count].active = 1;
    s_count++; return s_count - 1;
}
void ItemSpawn_Update(void) { }
void ItemSpawn_Remove(u32 index) { if (index < s_count) g_itemspawns[index].active = 0; }
u32 ItemSpawn_GetCount(void) { return s_count; }
void ItemSpawn_Clear(void) { s_count = 0; }
void ItemSpawn_Reset(void) { s_count = 0; }
