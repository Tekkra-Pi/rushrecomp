#include "nds_types.h"
void HazardHelper_Init(void) { }
s32 HazardHelper_CheckHit(s32 x, s32 y, s32 w, s32 h) {
    u32 i; (void)w; (void)h;
    for (i = 0; i < g_spike_count; i++) {
        if (!g_spikes[i].active) continue;
        s32 dx = x - g_spikes[i].x, dy = y - g_spikes[i].y;
        if (dx >= 0 && dx <= g_spikes[i].w && dy >= 0 && dy <= g_spikes[i].h) return 1;
    }
    for (i = 0; i < g_lava_count; i++) {
        if (!g_lavas[i].active) continue;
        s32 dx = x - g_lavas[i].x, dy = y - g_lavas[i].y;
        if (dx >= 0 && dx <= g_lavas[i].w && dy >= 0 && dy <= g_lavas[i].h) return 2;
    }
    return 0;
}
void HazardHelper_Update(void) { }
void HazardHelper_Clear(void) { }
void HazardHelper_Reset(void) { }
