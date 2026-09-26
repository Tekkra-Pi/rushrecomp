/* Gameplay level affine transform functions. */

#include "nds_types.h"

static u32 s_affine_count;

s32 AffineTransform_Add(s32 x, s32 y, u32 param) {
    if (s_affine_count >= 16) return -1;
    u32 i = s_affine_count;
    s_affine_count++;
    return i;
}

void AffineTransform_Update(void) { }

void AffineTransform_Remove(u32 index) {
    (void)index;
}

u32 AffineTransform_GetCount(void) { return s_affine_count; }

void AffineTransform_Clear(void) { s_affine_count = 0; }
void AffineTransform_Reset(void) { s_affine_count = 0; }
