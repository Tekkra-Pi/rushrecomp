/* Gameplay level boss defeat functions. */

#include "nds_types.h"

extern void Display_FadeToBlack(void);
extern void Sound_Play(u32 sfx);

#define BOSS_DEFEAT_WAIT 120

static u32 s_bossdefeat_state;
static u32 s_bossdefeat_timer;

void BossDefeat_Init(void) {
    s_bossdefeat_state = 0;
    s_bossdefeat_timer = 0;
    g_bossdefeat_state = 0;
}

void BossDefeat_Start(void) {
    s_bossdefeat_state = 1;
    s_bossdefeat_timer = 0;
    g_bossdefeat_state = 1;
    Sound_Play(0x100);
}

s32 BossDefeat_Update(void) {
    switch (s_bossdefeat_state) {
        case 0:
            return 0;
        case 1:
            s_bossdefeat_timer++;
            if (s_bossdefeat_timer >= BOSS_DEFEAT_WAIT) {
                s_bossdefeat_state = 2;
                s_bossdefeat_timer = 0;
                Display_FadeToBlack();
            }
            break;
        case 2:
            s_bossdefeat_timer++;
            if (s_bossdefeat_timer >= 60) {
                s_bossdefeat_state = 0;
                g_bossdefeat_state = 0;
                return 1;
            }
            break;
    }
    return 0;
}

void BossDefeat_Reset(void) {
    s_bossdefeat_state = 0;
    s_bossdefeat_timer = 0;
    g_bossdefeat_state = 0;
}
