#include "nds_types.h"
void Popup_Start(void) { g_popup_state = 1; g_popup_str = 0; }
s32 Popup_Update(void) {
    if (g_popup_state == 0) return 0;
    g_msgdisp_timer++;
    if (g_msgdisp_timer >= 120) { g_popup_state = 0; return 1; }
    return 0;
}
void Popup_Skip(void) { g_popup_state = 0; }
u32 Popup_GetState(void) { return g_popup_state; }
void Popup_Reset(void) { g_popup_state = 0; g_popup_str = 0; }
