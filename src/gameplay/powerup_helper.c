/* Gameplay level powerup helper functions. */
#include "nds_types.h"


void PowerupHelper_Init(void) { g_poweruphelper_active = 0; }

void PowerupHelper_Give(u32 type) {
    g_poweruphelper_active = 1;
    g_poweruphelper_type = type;
    switch (type) {
        case 0: /* speed shoes */
            Player_SetSpeedShoes(1);
            g_poweruphelper_timer = 600;
            break;
        case 1: /* invincibility */
            Player_SetInvincible(1);
            g_poweruphelper_timer = 600;
            break;
        case 2: /* shield */
            Player_SetShield(1);
            g_poweruphelper_timer = -1;
            break;
    }
    SoundEffect_Play(0x1a, 0x40, 0x100);
}

void PowerupHelper_Update(void) {
    if (!g_poweruphelper_active) return;
    if (g_poweruphelper_timer > 0) {
        g_poweruphelper_timer--;
        if (g_poweruphelper_timer <= 0) {
            switch (g_poweruphelper_type) {
                case 0: Player_SetSpeedShoes(0); break;
                case 1: Player_SetInvincible(0); break;
            }
            g_poweruphelper_active = 0;
        }
    }
}

void PowerupHelper_Remove(void) { g_poweruphelper_active = 0; }
s32 PowerupHelper_IsActive(void) { return g_poweruphelper_active; }
u32 PowerupHelper_GetType(void) { return g_poweruphelper_type; }
s32 PowerupHelper_GetTimer(void) { return g_poweruphelper_timer; }
void PowerupHelper_Reset(void) { g_poweruphelper_active = 0; g_poweruphelper_timer = 0; }
