/* Gameplay level event flag functions. */
#include "nds_types.h"

void EventFlag_Init(void) { }

void EventFlag_Set(u32 index) {
    if (index < 32) g_evtflags[index] = 1;
}

void EventFlag_Clear(u32 index) {
    if (index < 32) g_evtflags[index] = 0;
}

u32 EventFlag_Get(u32 index) {
    if (index >= 32) return 0;
    return g_evtflags[index];
}

void EventFlag_Reset(void) {
    u32 i;
    for (i = 0; i < 32; i++) g_evtflags[i] = 0;
}
