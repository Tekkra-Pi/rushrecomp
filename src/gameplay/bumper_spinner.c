/* Bumper and spinner functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Player_SetVX(s32 vx);
extern void Player_SetVY(s32 vy);

static BouncePadEntry s_bumper_entries[16];

void Bumper_Init(void) {
    g_bumperpad_count = 0;
}

void Bumper_Update(void) {
}

void Bumper_Bounce(void) {
    u32 i;
    for (i = 0; i < g_bumperpad_count; i++) {
        if (!s_bumper_entries[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - s_bumper_entries[i].x;
        s32 dy = py - s_bumper_entries[i].y;
        if (dx >= -0x300 && dx <= 0x300 && dy >= -0x300 && dy <= 0x300) {
            Player_SetVX((dx > 0) ? (s32)g_bumperpad_force : -(s32)g_bumperpad_force);
            Player_SetVY((dy > 0) ? (s32)g_bumperpad_force : -(s32)g_bumperpad_force);
        }
    }
}

void Spinner_Init(void) { }

void Spinner_Update(void) {
}

void Spinner_Launch(void) {
    Player_SetVY(-0x800);
}
