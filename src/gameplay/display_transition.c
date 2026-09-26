/* Gameplay level display transition functions. */

#include "nds_types.h"

static u32 s_disptrans_state;
static u32 s_disptrans_timer;

void DisplayTransition_Init(void) {
    s_disptrans_state = 0;
    s_disptrans_timer = 0;
    g_screentrans_state = 0;
    g_transition_type = 0;
    g_transition_timer = 0;
}

void DisplayTransition_Start(void) {
    s_disptrans_state = 1;
    s_disptrans_timer = 0;
    g_screentrans_state = 1;
}

s32 DisplayTransition_Update(void) {
    switch (s_disptrans_state) {
        case 0:
            return 0;
        case 1:
            s_disptrans_timer++;
            g_transition_timer = s_disptrans_timer;
            if (s_disptrans_timer >= 30) {
                s_disptrans_state = 2;
                s_disptrans_timer = 0;
            }
            break;
        case 2:
            s_disptrans_timer++;
            if (s_disptrans_timer >= 30) {
                s_disptrans_state = 0;
                g_screentrans_state = 0;
                return 1;
            }
            break;
    }
    return 0;
}

u32 DisplayTransition_GetState(void) { return s_disptrans_state; }

void DisplayTransition_Reset(void) {
    s_disptrans_state = 0;
    s_disptrans_timer = 0;
    g_screentrans_state = 0;
}
