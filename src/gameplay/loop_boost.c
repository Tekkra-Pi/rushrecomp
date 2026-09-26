#include "nds_types.h"
static u32 s_active;
void LoopBoost_Init(void) { s_active = 0; }
void LoopBoost_Start(void) {
    s_active = 1;
    g_boost_level += 0x20;
    if (g_boost_level > g_boost_max) g_boost_level = g_boost_max;
}
void LoopBoost_Update(void) { s_active = 0; }
s32 LoopBoost_IsActive(void) { return s_active; }
void LoopBoost_Reset(void) { s_active = 0; }
