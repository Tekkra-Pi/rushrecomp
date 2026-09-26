#include "nds_types.h"
void MsgDisp_Init(void) { g_msgdisp_str = 0; g_msgdisp_timer = 0; }
void MsgDisp_Show(u32 str_id) { g_msgdisp_str = str_id; g_msgdisp_timer = 0; }
void MsgDisp_Update(void) {
    if (g_msgdisp_str == 0) return;
    g_msgdisp_timer++; if (g_msgdisp_timer >= 120) g_msgdisp_str = 0;
}
void MsgDisp_Draw(void) { if (g_msgdisp_str) Display_PrintFixed(16, 160, "MSG"); }
void MsgDisp_Hide(void) { g_msgdisp_str = 0; }
s32 MsgDisp_IsVisible(void) { return g_msgdisp_str != 0 ? 1 : 0; }
void MsgDisp_Reset(void) { g_msgdisp_str = 0; g_msgdisp_timer = 0; }
