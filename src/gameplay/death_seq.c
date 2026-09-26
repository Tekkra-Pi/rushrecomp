/* Gameplay level death sequence functions. */

#include "nds_types.h"

extern void Player_GetPosition(s32 *x, s32 *y);
extern void Display_FadeToBlack(void);
extern void Sound_Play(u32 sfx);

static u32 s_deathseq_state;
static u32 s_deathseq_local_timer;

void DeathSeq_Init(void) {
    s_deathseq_state = 0;
    s_deathseq_local_timer = 0;
    g_deathseq_timer = 0;
}

void DeathSeq_Start(void) {
    s_deathseq_state = 1;
    s_deathseq_local_timer = 0;
    g_deathseq_timer = 0;
    Sound_Play(0x11);
}

s32 DeathSeq_Update(void) {
    switch (s_deathseq_state) {
        case 0:
            return 0;
        case 1:
            s_deathseq_local_timer++;
            g_deathseq_timer = s_deathseq_local_timer;
            if (s_deathseq_local_timer >= 30) {
                Display_FadeToBlack();
                s_deathseq_state = 2;
                s_deathseq_local_timer = 0;
            }
            break;
        case 2:
            s_deathseq_local_timer++;
            g_deathseq_timer = s_deathseq_local_timer;
            if (s_deathseq_local_timer >= 60) {
                s_deathseq_state = 0;
                return 1;
            }
            break;
    }
    return 0;
}

void DeathSeq_Draw(void) {
}

u32 DeathSeq_GetState(void) { return s_deathseq_state; }
u32 DeathSeq_GetTimer(void) { return s_deathseq_local_timer; }

void DeathSeq_Reset(void) {
    s_deathseq_state = 0;
    s_deathseq_local_timer = 0;
    g_deathseq_timer = 0;
}
