#include "nds_types.h"
static u32 s_state, s_timer;
void QuitSeq_Init(void) { s_state = 0; s_timer = 0; }
void QuitSeq_Start(void) { s_state = 1; s_timer = 0; }
s32 QuitSeq_Update(void) {
    if (s_state == 0) return 0;
    s_timer++; if (s_timer >= 60) { s_state = 0; return 1; } return 0;
}
void QuitSeq_Draw(void) { }
u32 QuitSeq_GetState(void) { return s_state; }
u32 QuitSeq_GetTimer(void) { return s_timer; }
void QuitSeq_Reset(void) { s_state = 0; s_timer = 0; }
