/* Camera lock functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_camlock_x_locked;
static u32 s_camlock_y_locked;

void CameraLock_Init(void) {
    s_camlock_x_locked = 0;
    s_camlock_y_locked = 0;
    g_camlock_lock_x = 0;
    g_camlock_lock_y = 0;
}

void CameraLock_LockX(void) {
    s_camlock_x_locked = 1;
    g_camlock_lock_x = 1;
    g_camlock_x = g_camera_x;
}

void CameraLock_LockY(void) {
    s_camlock_y_locked = 1;
    g_camlock_lock_y = 1;
    g_camlock_y = g_camera_y;
}

void CameraLock_SetPos(u32 index, u32 value) {
    (void)index;
    g_camlock_x = (s32)value;
}

void CameraLock_Update(void) {
}

s32 CameraLock_IsLockedX(void) { return s_camlock_x_locked; }
s32 CameraLock_IsLockedY(void) { return s_camlock_y_locked; }

void CameraLock_Reset(void) {
    s_camlock_x_locked = 0;
    s_camlock_y_locked = 0;
    g_camlock_lock_x = 0;
    g_camlock_lock_y = 0;
}
