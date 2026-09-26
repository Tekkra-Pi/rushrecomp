/* Player input functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u16 g_key_current;
extern u16 g_key_previous;
extern u16 g_key_pressed;
extern u16 g_key_released;
extern u16 g_key_held;

void PlayerInput_Init(void) {
    g_key_current = 0;
    g_key_previous = 0;
    g_key_pressed = 0;
    g_key_released = 0;
    g_key_held = 0;
}

void PlayerInput_Update(void) {
    g_key_pressed = g_key_current & ~g_key_previous;
    g_key_released = ~g_key_current & g_key_previous;
    g_key_held = g_key_current;
}

void PlayerInput_Process(void) {
    PlayerInput_Update();
}

u32 PlayerInput_GetHeld(void) {
    return (u32)g_key_held;
}

u32 PlayerInput_GetPressed(void) {
    return (u32)g_key_pressed;
}

u32 PlayerInput_GetReleased(void) {
    return (u32)g_key_released;
}

u32 PlayerInput_GetFlags(void) {
    return (u32)g_key_current;
}

s32 PlayerInput_CheckFlag(void) {
    return g_key_current != 0 ? 1 : 0;
}

s32 PlayerInput_IsLeft(void) {
    return (g_key_current & KEY_DLEFT) ? 1 : 0;
}

s32 PlayerInput_IsRight(void) {
    return (g_key_current & KEY_DRIGHT) ? 1 : 0;
}

s32 PlayerInput_IsUp(void) {
    return (g_key_current & KEY_DUP) ? 1 : 0;
}

s32 PlayerInput_IsDown(void) {
    return (g_key_current & KEY_DDOWN) ? 1 : 0;
}

s32 PlayerInput_IsJump(void) {
    return (g_key_pressed & KEY_A) ? 1 : 0;
}
