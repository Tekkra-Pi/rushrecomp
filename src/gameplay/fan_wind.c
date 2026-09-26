/* Gameplay level fan wind functions. */
#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Player_SetVY(s32 vy);

void FanWind_Init(void) { }

void FanWind_Update(void) {
}

void FanWind_Apply(void) {
    s32 px = Player_GetX();
    s32 py = Player_GetY();
    (void)px; (void)py;
}

void FanWind_Clear(void) { }
void FanWind_Reset(void) { }
