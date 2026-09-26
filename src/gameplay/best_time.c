/* Best time display functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_besttime_state;
static u32 s_besttime_timer;

void BestTime_Init(void) {
    s_besttime_state = 0;
    s_besttime_timer = 0;
}

void BestTime_Start(void) {
    s_besttime_state = 1;
    s_besttime_timer = 0;
}

s32 BestTime_Update(void) {
    if (s_besttime_state == 0) return 0;
    s_besttime_timer++;
    if (s_besttime_timer >= 120) {
        s_besttime_state = 0;
        return 1;
    }
    return 0;
}

void BestTime_Draw(void) {
}

void BestTime_Skip(void) {
    s_besttime_state = 0;
}

u32 BestTime_GetState(void) { return s_besttime_state; }

void BestTime_Reset(void) {
    s_besttime_state = 0;
    s_besttime_timer = 0;
}
