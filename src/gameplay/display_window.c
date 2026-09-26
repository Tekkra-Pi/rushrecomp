/* Gameplay level display window functions. */
#include "nds_types.h"


void DisplayWindow_Init(void) { g_dispwin_active = 0; }

void DisplayWindow_Set(s32 x1, s32 y1, s32 x2, s32 y2) {
    g_dispwin_active = 1;
    g_dispwin_x1 = x1; g_dispwin_y1 = y1;
    g_dispwin_x2 = x2; g_dispwin_y2 = y2;
    WinSet(0, x1, y1, x2, y2);
}

void DisplayWindow_Disable(void) { g_dispwin_active = 0; WinSet(0, 0, 0, 255, 192); }

s32 DisplayWindow_IsActive(void) { return g_dispwin_active; }
void DisplayWindow_Reset(void) { DisplayWindow_Init(); }
