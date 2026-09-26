/* Zone mode state machine functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 711-737 */

#include "nds_types.h"

extern u32 g_modestate_mode;
extern u32 g_gamestate_state;
extern u32 g_gamestate_prev;

u32 ModeState_Process(void) {
    u32 state = g_modestate_mode;
    g_gamestate_prev = g_gamestate_state;
    g_gamestate_state = state;
    return state;
}

u32 ModeState_GetCurrent(void) {
    return g_modestate_mode;
}

void Mode_EnterGameplay(void) {
    g_modestate_mode = 0;
    g_gamestate_state = 0;
}

void Mode_EnterMenu(void) {
    g_modestate_mode = 1;
    g_gamestate_prev = g_gamestate_state;
    g_gamestate_state = 1;
}

void Mode_EnterTransition(void) {
    g_modestate_mode = 2;
    g_gamestate_prev = g_gamestate_state;
    g_gamestate_state = 2;
}
