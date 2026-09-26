/* Gameplay level camera mode functions. */
#include "nds_types.h"


void CamMode_Init(void) { g_cammode_mode = 0; }

void CamMode_Set(u32 mode) { g_cammode_mode = mode; }
u32 CamMode_Get(void) { return g_cammode_mode; }

void CamMode_Update(void) {
    s32 px, py;
    Player_GetPosition(&px, &py);
    switch (g_cammode_mode) {
        case 0: /* follow */
            CamLerp_SetTarget(px, py);
            break;
        case 1: /* lock X */
            CamLock_SetX(px);
            break;
        case 2: /* lock Y */
            CamLock_SetY(py);
            break;
        case 3: /* fixed */
            break;
    }
}

void CamMode_Reset(void) { g_cammode_mode = 0; }
