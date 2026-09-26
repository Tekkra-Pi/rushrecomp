/* Camera cutscene functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_ccut_active;
static u32 s_ccut_timer;
static u32 s_ccut_index;

void CameraCutscene_Init(void) {
    s_ccut_active = 0;
    s_ccut_timer = 0;
    s_ccut_index = 0;
}

void CameraCutscene_Start(void) {
    s_ccut_active = 1;
    s_ccut_timer = 0;
    g_camcutscene_active = 1;
}

s32 CameraCutscene_Update(void) {
    if (!s_ccut_active) return 0;
    s_ccut_timer++;
    return 0;
}

void CameraCutscene_Stop(void) {
    s_ccut_active = 0;
    g_camcutscene_active = 0;
}

s32 CameraCutscene_IsActive(u32 index) {
    (void)index;
    return s_ccut_active;
}

u32 CameraCutscene_GetTimer(void) { return s_ccut_timer; }

u32 CameraCutscene_GetIndex(void) { return s_ccut_index; }

void CameraCutscene_SetPath(u32 index, u32 value) {
    (void)index;
    s_ccut_index = value;
    g_camcut_path = value;
}

void CameraCutscene_Reset(void) {
    s_ccut_active = 0;
    s_ccut_timer = 0;
    s_ccut_index = 0;
    g_camcutscene_active = 0;
}
