#include "nds_types.h"
void ScreenWipe_Init(void) { g_wipe_type = 0; g_wipe_progress = 0; }
void ScreenWipe_Start(void) { g_wipe_type = 1; g_wipe_progress = 0; }
s32 ScreenWipe_Update(void) {
    if (g_wipe_type == 0) return 0;
    g_wipe_progress++;
    if (g_wipe_progress >= 60) { g_wipe_type = 0; return 1; }
    return 0;
}
void ScreenWipe_Draw(void) { }
void ScreenWipe_Stop(void) { g_wipe_type = 0; }
s32 ScreenWipe_IsActive(u32 index) { (void)index; return g_wipe_type; }
u32 ScreenWipe_GetType(void) { return g_wipe_type; }
u32 ScreenWipe_GetProgress(void) { return g_wipe_progress; }
void ScreenWipe_Reset(void) { g_wipe_type = 0; g_wipe_progress = 0; }
