/* Cam cutscene functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_camcut_active;
static u32 s_camcut_timer;

void CamCutscene_Init(void) {
    s_camcut_active = 0;
    s_camcut_timer = 0;
    g_camcutscene_active = 0;
}

void CamCutscene_Start(void) {
    s_camcut_active = 1;
    s_camcut_timer = 0;
    g_camcutscene_active = 1;
}

void CamCutscene_Update(void) {
    if (!s_camcut_active) return;
    s_camcut_timer++;
    g_camcut_path_index++;
}

s32 CamCutscene_IsActive(u32 index) {
    (void)index;
    return s_camcut_active;
}

void CamCutscene_Reset(void) {
    s_camcut_active = 0;
    s_camcut_timer = 0;
    g_camcutscene_active = 0;
    g_camcut_path_index = 0;
}
