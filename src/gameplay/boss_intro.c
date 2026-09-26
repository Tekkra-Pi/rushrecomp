/* Gameplay level boss intro functions. */

#include "nds_types.h"

extern void Display_FadeToBlack(void);
extern void Display_FadeFromBlack(void);
extern void Sound_Play(u32 sfx);

static u32 s_bossintro_state;
static u32 s_bossintro_timer;

void BossIntro_Init(void) {
    s_bossintro_state = 0;
    s_bossintro_timer = 0;
    g_bossintro_state = 0;
}

void BossIntro_Start(void) {
    s_bossintro_state = 1;
    s_bossintro_timer = 0;
    g_bossintro_state = 1;
    Display_FadeToBlack();
}

s32 BossIntro_Update(void) {
    switch (s_bossintro_state) {
        case 0:
            return 0;
        case 1:
            s_bossintro_timer++;
            if (s_bossintro_timer >= 30) {
                Display_FadeFromBlack();
                s_bossintro_state = 2;
                s_bossintro_timer = 0;
            }
            break;
        case 2:
            s_bossintro_timer++;
            if (s_bossintro_timer >= 90) {
                s_bossintro_state = 0;
                g_bossintro_state = 0;
                return 1;
            }
            break;
    }
    return 0;
}

void BossIntro_Reset(void) {
    s_bossintro_state = 0;
    s_bossintro_timer = 0;
    g_bossintro_state = 0;
}
