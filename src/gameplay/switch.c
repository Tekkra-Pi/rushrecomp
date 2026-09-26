/* Gameplay level switch functions. */
#include "nds_types.h"

extern void Sound_Play(u32 sfx);

#define SWITCH_MAX 16

void Switch_Init(void) { }

s32 Switch_Add(s32 x, s32 y, u32 param) {
    u32 i = 0;
    g_switches[i].x = x;
    g_switches[i].y = y;
    g_switches[i].state = 0;
    g_switches[i].active = 1;
    return i;
}

void Switch_Update(void) {
}

void Switch_Toggle(u32 index) {
    if (index >= SWITCH_MAX) return;
    g_switches[index].state = !g_switches[index].state;
    Sound_Play(0x12);
}

void Switch_Remove(u32 index) {
    if (index < SWITCH_MAX) g_switches[index].active = 0;
}

u32 Switch_GetState(u32 index) {
    if (index >= SWITCH_MAX) return 0;
    return g_switches[index].state;
}

u32 Switch_GetCount(void) { return SWITCH_MAX; }

void Switch_Clear(void) {
    u32 i;
    for (i = 0; i < SWITCH_MAX; i++) g_switches[i].active = 0;
}

void Switch_Reset(void) {
    u32 i;
    for (i = 0; i < SWITCH_MAX; i++) {
        g_switches[i].active = 0;
        g_switches[i].state = 0;
    }
}
