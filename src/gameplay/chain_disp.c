/* Gameplay level chain display functions. */

#include "nds_types.h"

static u32 s_chaindisp_active;
static u32 s_chaindisp_timer;

void ChainDisp_Init(void) {
    s_chaindisp_active = 0;
    s_chaindisp_timer = 0;
}

void ChainDisp_Start(void) {
    s_chaindisp_active = 1;
    s_chaindisp_timer = 0;
}

void ChainDisp_Update(void) {
    if (!s_chaindisp_active) return;
    s_chaindisp_timer++;
}

void ChainDisp_Draw(void) {
}

s32 ChainDisp_IsActive(u32 index) {
    (void)index;
    return s_chaindisp_active;
}

void ChainDisp_Reset(void) {
    s_chaindisp_active = 0;
    s_chaindisp_timer = 0;
}
