/* Gameplay pause functions. */
#include "nds_types.h"

static u32 s_state, s_menu_idx;

void Pause_Init(void) { s_state = 0; s_menu_idx = 0; }
void Pause_Update(void) {
    if (g_key_pressed & (1 << 3)) {
        if (s_state == 0) { s_state = 1; g_game_paused = 1; }
        else { s_state = 0; g_game_paused = 0; }
    }
    if (s_state == 1) {
        if (g_key_pressed & (1 << 5)) { if (s_menu_idx > 0) s_menu_idx--; }
        if (g_key_pressed & (1 << 4)) { if (s_menu_idx < 2) s_menu_idx++; }
    }
}
void Pause_Select(void) {
    if (s_state == 0) return;
    if (s_menu_idx == 1) { s_state = 0; g_game_paused = 0; }
    if (s_menu_idx == 2) { s_state = 0; g_game_paused = 0; g_system_flags |= 1; }
}
void Pause_Resume(void) { s_state = 0; g_game_paused = 0; }
void Pause_Draw(void) {
    if (s_state == 0) return;
    Display_PrintFixed(100, 60, "PAUSED");
    Display_PrintFixed(100, 80, "RESUME");
    Display_PrintFixed(100, 100, "RETRY");
    Display_PrintFixed(100, 120, "QUIT");
    Display_PrintFixed(80, 80 + s_menu_idx * 20, ">");
}
s32 Pause_IsActive(u32 index) { (void)index; return s_state; }
u32 Pause_GetState(void) { return s_state; }
u32 Pause_GetMenuIndex(void) { return s_menu_idx; }
void Pause_Force(void) { s_state = 1; g_game_paused = 1; }
void Pause_Reset(void) { s_state = 0; s_menu_idx = 0; g_game_paused = 0; }
