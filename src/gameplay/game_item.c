/* Gameplay item functions. */
#include "nds_types.h"

static u32 s_count;

void GameItem_Init(void) { s_count = 0; }
s32 GameItem_Add(s32 x, s32 y, u32 param) {
    if (s_count >= 32) return -1;
    g_itemboxes[s_count].x = x; g_itemboxes[s_count].y = y;
    g_itemboxes[s_count].type = param; g_itemboxes[s_count].active = 1;
    s_count++; return s_count - 1;
}
void GameItem_Remove(u32 index) { if (index < s_count) g_itemboxes[index].active = 0; }
s32 GameItem_Has(void) { return s_count > 0 ? 1 : 0; }
u32 GameItem_Get(void) { return s_count > 0 ? g_itemboxes[0].type : 0; }
u32 GameItem_GetCount(void) { return s_count; }
void GameItem_Clear(void) { s_count = 0; }
s32 GameItem_Use(void) {
    if (s_count == 0) return -1;
    u32 type = g_itemboxes[0].type;
    u32 i;
    for (i = 0; i < s_count - 1; i++) g_itemboxes[i] = g_itemboxes[i + 1];
    s_count--; return (s32)type;
}
