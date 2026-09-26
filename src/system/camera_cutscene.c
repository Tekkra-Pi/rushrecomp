/* Camera cutscene functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_camcutscene_active;
extern u32 g_camcut_path;
extern u32 g_camcut_path_index;
extern s32 g_camera_x, g_camera_y;

void CameraCutscene_Draw(void) {
    /* Nothing to draw - camera cutscene is state-only */
}

void CameraCutscene_LoadPath(void) {
    g_camcut_path_index = 0;
    g_camcutscene_active = 1;
}

void CameraCutscene_MoveAlongPath(void) {
    if (!g_camcutscene_active) return;
    extern s32 CamPath_GetPoint(u32 path, u32 index, s32 *x, s32 *y);
    s32 tx, ty;
    if (CamPath_GetPoint(g_camcut_path, g_camcut_path_index, &tx, &ty) == 0) {
        g_camera_x = tx;
        g_camera_y = ty;
        g_camcut_path_index++;
    } else {
        g_camcutscene_active = 0;
    }
}

s32 CameraCutscene_CheckComplete(void) {
    return g_camcutscene_active ? 0 : 1;
}
