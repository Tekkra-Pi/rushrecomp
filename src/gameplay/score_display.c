#include "nds_types.h"
static u32 s_vis;
void ScoreDisplay_Init(void) { s_vis = 0; g_scoredisplay_visible = 0; }
void ScoreDisplay_SetPosition(u32 index, u32 value) { (void)index; g_scoredisplay_x = (s32)value; }
void ScoreDisplay_Update(void) {
    if (!s_vis) return;
    if (g_scoredisplay_score < g_scoredisplay_target) {
        g_scoredisplay_score += 100;
        if (g_scoredisplay_score > g_scoredisplay_target) g_scoredisplay_score = g_scoredisplay_target;
    }
}
void ScoreDisplay_Draw(void) { if (s_vis) Display_PrintFixed(g_scoredisplay_x, g_scoredisplay_y, "SCORE"); }
void ScoreDisplay_Show(void) { s_vis = 1; g_scoredisplay_visible = 1; }
void ScoreDisplay_Hide(void) { s_vis = 0; g_scoredisplay_visible = 0; }
s32 ScoreDisplay_IsVisible(void) { return s_vis; }
u32 ScoreDisplay_GetScore(void) { return g_scoredisplay_score; }
void ScoreDisplay_SetScore(u32 index, u32 value) { (void)index; g_scoredisplay_target = value; }
void ScoreDisplay_Reset(void) { s_vis = 0; g_scoredisplay_visible = 0; g_scoredisplay_score = 0; }
