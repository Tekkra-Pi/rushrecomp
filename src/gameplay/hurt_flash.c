/* Gameplay level hurt flash functions. */
#include "nds_types.h"

void HurtFlash_Init(void) {
    g_hurtflash_alpha = 0;
    g_hurtflash_timer = 0;
}

void HurtFlash_Start(void) {
    g_hurtflash_alpha = 16;
    g_hurtflash_timer = 20;
}

void HurtFlash_Update(void) {
    if (g_hurtflash_timer > 0) {
        g_hurtflash_timer--;
        if (g_hurtflash_timer & 2) g_hurtflash_alpha = 16;
        else g_hurtflash_alpha = 0;
    } else {
        g_hurtflash_alpha = 0;
    }
}

s32 HurtFlash_IsActive(void) {
    return g_hurtflash_timer > 0;
}

void HurtFlash_Stop(void) {
    g_hurtflash_timer = 0;
    g_hurtflash_alpha = 0;
}

void HurtFlash_Reset(void) {
    g_hurtflash_alpha = 0;
    g_hurtflash_timer = 0;
}
