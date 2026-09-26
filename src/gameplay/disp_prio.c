/* Display priority helper functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_dispprio_count;

void DispPrio_Init(void) { s_dispprio_count = 0; }

void DispPrio_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y; (void)param;
    if (s_dispprio_count < 64) s_dispprio_count++;
}

void DispPrio_Sort(void) {
}

void DispPrio_Draw(void) {
}

u32 DispPrio_GetCount(void) { return s_dispprio_count; }

void DispPrio_Clear(void) { s_dispprio_count = 0; }
void DispPrio_Reset(void) { s_dispprio_count = 0; }
