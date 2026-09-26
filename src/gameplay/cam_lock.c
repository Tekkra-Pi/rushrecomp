/* Cam lock functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_camlockx_val;
static u32 s_camlocky_val;

void CamLock_Init(void) {
    s_camlockx_val = 0;
    s_camlocky_val = 0;
}

void CamLock_SetX(u32 index, u32 value) {
    (void)index;
    s_camlockx_val = value;
    g_camlock_x = value;
}

void CamLock_SetY(u32 index, u32 value) {
    (void)index;
    s_camlocky_val = value;
    g_camlock_y = value;
}

void CamLock_UnlockX(void) { g_camlock_lock_x = 0; }
void CamLock_UnlockY(void) { g_camlock_lock_y = 0; }
void CamLock_UnlockAll(void) {
    g_camlock_lock_x = 0;
    g_camlock_lock_y = 0;
}

void CamLock_Update(void) { }

void CamLock_Reset(void) {
    s_camlockx_val = 0;
    s_camlocky_val = 0;
    g_camlock_lock_x = 0;
    g_camlock_lock_y = 0;
}
