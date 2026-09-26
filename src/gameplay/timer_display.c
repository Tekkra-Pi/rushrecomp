#include "nds_types.h"
static u32 s_vis;
void TimerDisplay_Init(void) { s_vis = 0; g_timerdisplay_visible = 0; }
void TimerDisplay_SetPosition(u32 index, u32 value) { (void)index; g_timerdisplay_x = (s32)value; }
void TimerDisplay_Update(void) { }
void TimerDisplay_Draw(void) { if (s_vis) Display_PrintFixed(g_timerdisplay_x, g_timerdisplay_y, "TIME"); }
void TimerDisplay_Show(void) { s_vis = 1; g_timerdisplay_visible = 1; }
void TimerDisplay_Hide(void) { s_vis = 0; g_timerdisplay_visible = 0; }
s32 TimerDisplay_IsVisible(void) { return s_vis; }
u32 TimerDisplay_GetTime(void) { return g_game_time; }
void TimerDisplay_SetTime(u32 index, u32 value) { (void)index; (void)value; }
void TimerDisplay_Reset(void) { s_vis = 0; g_timerdisplay_visible = 0; }
