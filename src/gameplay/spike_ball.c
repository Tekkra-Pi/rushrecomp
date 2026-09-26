/* Gameplay level spike ball functions. */
#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

void SpikeBall_Init(void) { }

void SpikeBall_Update(void) {
    u32 i;
    for (i = 0; i < g_spike_count; i++) {
        if (!g_spikes[i].active) continue;
        g_spikes[i].y += 0x10;
    }
}

u32 SpikeBall_GetCount(void) { return g_spike_count; }

void SpikeBall_Clear(void) { g_spike_count = 0; }
void SpikeBall_Reset(void) { g_spike_count = 0; }
