/* Display transition functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_screentrans_state;
extern u32 g_transition_type;
extern u32 g_transition_timer;
extern u32 g_transition_duration;
extern u32 g_disptrans_state;

void DisplayTransition_Init(void) {
    g_screentrans_state = 0;
    g_transition_type = 0;
    g_transition_timer = 0;
    g_transition_duration = 0;
    g_disptrans_state = 0;
}

void DisplayTransition_Play(u32 index) {
    g_transition_type = index;
    g_transition_timer = 0;
    g_transition_duration = 16;
    g_screentrans_state = 1;
}

void DisplayTransition_Update(void) {
    if (g_screentrans_state) {
        if (g_transition_timer < g_transition_duration) {
            g_transition_timer++;
        } else {
            g_screentrans_state = 0;
        }
    }
}

void DisplayTransition_FadeOut(void) {
    DisplayTransition_Play(0);
}

void DisplayTransition_FadeIn(void) {
    DisplayTransition_Play(1);
}

void DisplayTransition_SlideLeft(void) {
    DisplayTransition_Play(2);
}

void DisplayTransition_SlideRight(void) {
    DisplayTransition_Play(3);
}

void DisplayTransition_Iris(void) {
    DisplayTransition_Play(4);
}

s32 DisplayTransition_IsComplete(u32 index) {
    (void)index;
    return g_screentrans_state == 0 ? 1 : 0;
}

void DisplayTransition_Cancel(void) {
    g_screentrans_state = 0;
    g_transition_timer = 0;
}
