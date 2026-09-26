/* Gameplay level cutscene functions. */

#include "nds_types.h"

void Cutscene_Init(void) {
    g_cutscene_state = 0;
    g_cutscene_timer = 0;
}

s32 Cutscene_Update(void) {
    if (g_cutscene_state == 0) return 0;
    g_cutscene_timer++;
    return 0;
}

void Cutscene_Draw(void) {
}

void Cutscene_LoadData(void) {
    g_cutscene_state = 1;
    g_cutscene_timer = 0;
}
