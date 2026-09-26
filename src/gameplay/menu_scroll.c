#include "nds_types.h"
void MenuScroll_Init(void) { g_menuscroll_offset = 0; g_menuscroll_speed = 0; }
void MenuScroll_SetSpeed(s32 speed) { g_menuscroll_speed = speed; }
void MenuScroll_Update(void) { g_menuscroll_offset += g_menuscroll_speed; }
s32 MenuScroll_GetOffset(void) { return g_menuscroll_offset; }
void MenuScroll_Reset(void) { g_menuscroll_offset = 0; g_menuscroll_speed = 0; }
