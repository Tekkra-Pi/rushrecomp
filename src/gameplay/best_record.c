/* Best record functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_bestrecord_score;
static u32 s_bestrecord_time;
static u32 s_bestrecord_rings;

void BestRecord_Init(void) {
    s_bestrecord_score = 0;
    s_bestrecord_time = 0;
    s_bestrecord_rings = 0;
    g_bestrecord_score = 0;
    g_bestrecord_time = 0;
    g_bestrecord_rings = 0;
}

s32 BestRecord_Check(void) {
    if (g_game_score > g_bestrecord_score) {
        g_bestrecord_score = g_game_score;
        return 1;
    }
    if (g_game_rings > g_bestrecord_rings) {
        g_bestrecord_rings = g_game_rings;
    }
    return 0;
}

void BestRecord_Draw(void) {
}

u32 BestRecord_GetScore(void) { return g_bestrecord_score; }
u32 BestRecord_GetTime(void) { return g_bestrecord_time; }
u32 BestRecord_GetRings(void) { return g_bestrecord_rings; }

void BestRecord_Reset(void) {
    s_bestrecord_score = 0;
    s_bestrecord_time = 0;
    s_bestrecord_rings = 0;
}
