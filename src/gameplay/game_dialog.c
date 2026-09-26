/* Gameplay dialog functions. */
#include "nds_types.h"

void Dialog_Init(void) { g_dialog_state = 0; g_dialog_char_index = 0; }
void Dialog_Start(void) { g_dialog_state = 1; g_dialog_char_index = 0; }
s32 Dialog_Update(void) {
    if (g_dialog_state == 0) return 0;
    g_dialog_char_index++;
    if (g_key_pressed & (1 << 0)) { g_dialog_state = 0; return 1; }
    return 0;
}
void Dialog_Draw(void) { if (g_dialog_state) Display_PrintFixed(16, 160, "DIALOG"); }
s32 Dialog_IsComplete(u32 index) { (void)index; return g_dialog_state == 0 ? 1 : 0; }
u32 Dialog_GetCharIndex(void) { return g_dialog_char_index; }
u32 Dialog_GetState(void) { return g_dialog_state; }
void Dialog_Stop(void) { g_dialog_state = 0; }
void Dialog_Reset(void) { g_dialog_state = 0; g_dialog_char_index = 0; }
