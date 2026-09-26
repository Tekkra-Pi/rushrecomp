#include "nds_types.h"
static u32 s_count;
void TrickChain_Init(void) { s_count = 0; g_trickchain_score = 0; }
void TrickChain_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y;
    if (s_count >= 8) return;
    g_trickchain_tricks[s_count] = param;
    g_trickchain_scores[s_count] = param * 100;
    g_trickchain_score += param * 100;
    s_count++;
}
void TrickChain_Update(void) { }
void TrickChain_Reset(void) { s_count = 0; g_trickchain_score = 0; }
u32 TrickChain_GetCount(void) { return s_count; }
u32 TrickChain_GetScore(void) { return g_trickchain_score; }
u32 TrickChain_GetTimer(void) { return 0; }
u32 TrickChain_GetProgress(void) { return s_count; }
u32 TrickChain_GetBonus(void) { return g_trickchain_score; }
s32 TrickChain_IsActive(u32 index) { (void)index; return s_count > 0 ? 1 : 0; }
u32 TrickChain_GetTrickId(void) { return s_count > 0 ? g_trickchain_tricks[0] : 0; }
u32 TrickChain_GetTrickScore(void) { return s_count > 0 ? g_trickchain_scores[0] : 0; }
void TrickChain_RemoveLast(void) { if (s_count > 0) s_count--; }
