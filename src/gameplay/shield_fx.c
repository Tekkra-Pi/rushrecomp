/* Gameplay level shield bubble FX functions. */
#include "nds_types.h"


void ShieldFX_Init(void) { g_shieldfx_active = 0; }

void ShieldFX_Start(void) { g_shieldfx_active = 1; g_shieldfx_timer = 0; }
void ShieldFX_Stop(void) { g_shieldfx_active = 0; }

void ShieldFX_Update(void) {
    if (!g_shieldfx_active) return;
    g_shieldfx_timer++;
}

void ShieldFX_Draw(void) {
    if (!g_shieldfx_active) return;
    s32 px, py;
    Player_GetPosition(&px, &py);
    u32 tile = 0x404 + (g_shieldfx_timer / 4 % 4);
    OAM_AddSprite(px >> 8 - 16, py >> 8 - 16, tile, 0x3000);
}

s32 ShieldFX_IsActive(void) { return g_shieldfx_active; }
void ShieldFX_Reset(void) { g_shieldfx_active = 0; g_shieldfx_timer = 0; }
