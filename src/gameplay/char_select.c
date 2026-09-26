/* Gameplay level character select functions. */
#include "nds_types.h"

static u32 s_sel, s_state, s_timer;

void CharSelect_Init(void) { s_sel = 0; s_state = 0; s_timer = 0; }
void CharSelect_Start(void) { s_state = 1; s_timer = 0; }

u32 CharSelect_Update(void) {
    if (s_state == 0) return 0;
    s_timer++;
    if (g_key_pressed & (1 << 0)) { s_state = 0; SoundEffect_Play(0xd0, 0x40, 0x100); return 1; }
    return 0;
}

void CharSelect_Draw(void) {
    if (s_state == 0) return;
    Display_PrintFixed(80, 80, "CHARACTER SELECT");
}

u32 CharSelect_GetSelection(void) { return s_sel; }
void CharSelect_Reset(void) { s_sel = 0; s_state = 0; s_timer = 0; }
