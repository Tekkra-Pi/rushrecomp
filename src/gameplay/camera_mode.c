/* Camera mode functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

void CameraMode_Init(void) {
    g_cammode_mode = 0;
}

void CameraMode_Set(u32 index, u32 value) {
    (void)index;
    g_cammode_mode = value;
}

void CameraMode_Update(void) {
}

void CameraMode_SetMode(u32 index, u32 value) {
    (void)index;
    g_cammode_mode = value;
}

u32 CameraMode_GetMode(void) { return g_cammode_mode; }

u32 CameraMode_GetTimer(void) { return 0; }

u32 CameraMode_GetParam(void) { return g_cammode_mode; }

void CameraMode_Reset(void) {
    g_cammode_mode = 0;
}
