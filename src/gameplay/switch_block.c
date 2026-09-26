/* Gameplay level switch block functions. */

#include "nds_types.h"

#define SWITCHBLOCK_MAX 16
#define LOCAL_SWITCH_MAX 16

void SwitchBlock_Init(void) { }

s32 SwitchBlock_Add(s32 x, s32 y, u32 switch_id) {
    u32 i = 0;
    g_switchblocks[i].x = x;
    g_switchblocks[i].y = y;
    g_switchblocks[i].switch_id = switch_id;
    g_switchblocks[i].active = 1;
    return i;
}

void SwitchBlock_Update(void) {
    u32 i;
    for (i = 0; i < SWITCHBLOCK_MAX; i++) {
        if (!g_switchblocks[i].active) continue;
        u32 sw = g_switchblocks[i].switch_id;
        if (sw < LOCAL_SWITCH_MAX) {
            g_switchblocks[i].active = 0;
        }
    }
}

void SwitchBlock_Remove(u32 index) {
    if (index < SWITCHBLOCK_MAX) g_switchblocks[index].active = 0;
}

u32 SwitchBlock_GetCount(void) { return SWITCHBLOCK_MAX; }

void SwitchBlock_Clear(void) {
    u32 i;
    for (i = 0; i < SWITCHBLOCK_MAX; i++) g_switchblocks[i].active = 0;
}

void SwitchBlock_Reset(void) {
    u32 i;
    for (i = 0; i < SWITCHBLOCK_MAX; i++) g_switchblocks[i].active = 0;
}
