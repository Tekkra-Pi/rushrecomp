/* Gameplay level input display functions. */
#include "nds_types.h"


void InputDisp_Init(void) { g_inputdisp_active = 0; }

void InputDisp_Start(void) { g_inputdisp_active = 1; }
void InputDisp_Stop(void) { g_inputdisp_active = 0; }

void InputDisp_Draw(void) {
    if (!g_inputdisp_active) return;
    u16 held = Input_GetHeld();
    Display_PrintFixed(200, 176, (held & KEY_UP) ? "U" : " ");
    Display_PrintFixed(200, 184, (held & KEY_DOWN) ? "D" : " ");
    Display_PrintFixed(192, 180, (held & KEY_LEFT) ? "L" : " ");
    Display_PrintFixed(208, 180, (held & KEY_RIGHT) ? "R" : " ");
    Display_PrintFixed(216, 176, (held & KEY_A) ? "A" : " ");
    Display_PrintFixed(224, 176, (held & KEY_B) ? "B" : " ");
}

s32 InputDisp_IsActive(void) { return g_inputdisp_active; }
void InputDisp_Reset(void) { g_inputdisp_active = 0; }
