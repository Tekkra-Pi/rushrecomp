/* Early ARM9 hardware init - math coprocessor. */
#include "nds_types.h"

void HwInit_MathCP(void) {
    /* Enable math coprocessor (PCMNT) */
    *(volatile u32 *)0x04000240 = 0x820F;
}

void HwInit_MathDiv(void) {
    /* Initialize hardware divider */
    *(volatile u64 *)0x04000280 = 0;
    *(volatile u32 *)0x04000290 = 0;
    *(volatile u32 *)0x04000294 = 0;
}

void HwInit_MathSqrt(void) {
    /* Initialize hardware square root */
    *(volatile u32 *)0x040002B0 = 0;
}
