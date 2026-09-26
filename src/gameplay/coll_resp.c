/* Gameplay level collision response helper functions. */
#include "nds_types.h"

static u32 s_type;

s32 CollResp_GetResponseType(void) { return s_type; }
void CollResp_OnHit(void) { Player_Hurt(); SoundEffect_Play(0xd2, 0x40, 0x100); }
s32 CollResp_ShouldCollide(void) { return 1; }
