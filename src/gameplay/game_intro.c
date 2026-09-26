/* Gameplay intro functions. */
#include "nds_types.h"

void Intro_Init(void) { g_intro_step = 0; g_cutscene_timer = 0; }
void Intro_Start(void) { g_intro_step = 1; g_cutscene_timer = 0; }
s32 Intro_Update(void) {
    if (g_intro_step == 0) return 0;
    g_cutscene_timer++;
    if (g_cutscene_timer >= 180) {
        g_intro_step++; g_cutscene_timer = 0;
        if (g_intro_step >= 5) { g_intro_step = 0; return 1; }
    }
    return 0;
}
void Intro_Draw(void) { }
s32 Intro_IsComplete(u32 index) { (void)index; return g_intro_step == 0 ? 1 : 0; }
u32 Intro_GetState(void) { return g_intro_step; }
u32 Intro_GetStep(void) { return g_intro_step; }
u32 Intro_GetTimer(void) { return g_cutscene_timer; }
void Intro_Skip(void) { g_intro_step = 0; }
void Intro_Reset(void) { g_intro_step = 0; g_cutscene_timer = 0; }
