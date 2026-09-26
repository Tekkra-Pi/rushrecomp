/* Gameplay level spike functions. */
#include "nds_types.h"


void Spike_Init(void) { g_spike_count = 0; }

s32 Spike_Add(s32 x, s32 y, u32 dir, u32 count) {
    if (g_spike_count >= 32) return -1;
    u32 i = g_spike_count;
    g_spikes[i].x = x; g_spikes[i].y = y;
    g_spikes[i].dir = dir; g_spikes[i].count = count;
    g_spikes[i].active = 1;
    g_spike_count++; return i;
}

void Spike_Update(void) {
    u32 i;
    for (i = 0; i < g_spike_count; i++) {
        if (!g_spikes[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_spikes[i].x;
        s32 dy = py - g_spikes[i].y;
        if (dx >= -(s32)g_spikes[i].count * 0x800 && dx <= (s32)g_spikes[i].count * 0x800 &&
            dy >= -0x400 && dy <= 0x400 && !Player_IsInvincible()) {
            Player_Hurt();
        }
    }
}

void Spike_Remove(u32 i) { if (i < g_spike_count) g_spike_count--; }
u32 Spike_GetCount(void) { return g_spike_count; }
void Spike_Clear(void) { g_spike_count = 0; }
void Spike_Reset(void) { g_spike_count = 0; }
