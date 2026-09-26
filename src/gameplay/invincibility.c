#include "nds_types.h"
void Invincibility_Init(void) { g_invinv_timer = 0; g_invinv_flicker = 0; }
void Invincibility_Start(void) { g_invinv_timer = 180; g_player_flags |= 1; }
void Invincibility_Update(void) {
    if (g_invinv_timer > 0) {
        g_invinv_timer--; g_invinv_flicker = (g_invinv_timer / 2) & 1;
        if (g_invinv_timer == 0) { g_player_flags &= ~1; g_invinv_flicker = 0; }
    }
}
s32 Invincibility_IsActive(void) { return g_invinv_timer > 0 ? 1 : 0; }
u32 Invincibility_GetTimer(void) { return g_invinv_timer; }
void Invincibility_Reset(void) { g_invinv_timer = 0; g_invinv_flicker = 0; g_player_flags &= ~1; }
