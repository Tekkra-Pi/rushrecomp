/* Player sound functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_sfx_volume;
extern u32 g_sfx_muted;

void PlayerSound_Init(void) {
    g_sfx_muted = 0;
}

void PlayerSound_Play(u32 index) {
    (void)index;
    if (g_sfx_muted) return;
    extern s32 SoundChannel_Alloc(void);
    SoundChannel_Alloc();
}

void PlayerSound_Stop(void) {
    extern void SoundChannel_Stop(void);
    SoundChannel_Stop();
}

void PlayerSound_PlayJump(void) {
    PlayerSound_Play(0);
}

void PlayerSound_PlayRoll(void) {
    PlayerSound_Play(1);
}

void PlayerSound_PlayDash(void) {
    PlayerSound_Play(2);
}

void PlayerSound_PlaySkid(void) {
    PlayerSound_Play(3);
}

void PlayerSound_PlayRing(void) {
    PlayerSound_Play(4);
}

void PlayerSound_PlayHurt(void) {
    PlayerSound_Play(5);
}

void PlayerSound_PlayDeath(void) {
    PlayerSound_Play(6);
}

void PlayerSound_PlayTrick(void) {
    PlayerSound_Play(7);
}

void PlayerSound_PlayBoost(void) {
    PlayerSound_Play(8);
}

void PlayerSound_StopAll(void) {
    PlayerSound_Stop();
}
