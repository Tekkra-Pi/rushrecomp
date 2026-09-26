/* Gameplay level boost chain functions. */
#include "nds_types.h"


void BoostChain_Init(void) { g_boostchain_count = 0; g_boostchain_timer = 0; }

void BoostChain_Add(void) {
    g_boostchain_count++;
    g_boostchain_timer = 90;
    Player_AddScore(g_boostchain_count * 100);
}

void BoostChain_Update(void) {
    if (g_boostchain_timer > 0) {
        g_boostchain_timer--;
    } else {
        g_boostchain_count = 0;
    }
}

u32 BoostChain_GetCount(void) { return g_boostchain_count; }
void BoostChain_Reset(void) { g_boostchain_count = 0; g_boostchain_timer = 0; }
