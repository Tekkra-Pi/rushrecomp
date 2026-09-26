/* Gameplay level dialogue functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

void Dialogue_Init(void) {
    g_dialog_state = 0;
    g_dialog_char_index = 0;
}

s32 Dialogue_Update(void) {
    if (g_dialog_state == 0) return 0;
    g_dialog_char_index++;
    return 0;
}

void Dialogue_Draw(void) {
}

void Dialogue_LoadTable(void) {
    g_dialog_state = 1;
    g_dialog_char_index = 0;
}

void Dialogue_UpdateText(void) {
    if (g_dialog_state == 0) return;
    g_dialog_char_index++;
}
