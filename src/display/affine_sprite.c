/* Affine sprite functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_oam_count;

#define MAX_AFFINE_SPRITES 32

s32 AffineSprite_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y; (void)param;
    if (g_oam_count >= 128) return -1;
    g_oam_count++;
    return (s32)(g_oam_count - 1);
}

void AffineSprite_SetTransform(u32 index, u32 value) {
    (void)index;
    (void)value;
}

void AffineSprite_Remove(u32 index) {
    (void)index;
    if (g_oam_count > 0) g_oam_count--;
}
