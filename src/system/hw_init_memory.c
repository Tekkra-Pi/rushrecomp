/* Early ARM9 hardware init - memory control. */
#include "nds_types.h"

void HwInit_MemoryCtrl(void) {
    /* Configure ARM9 memory wait states */
    *(volatile u32 *)0x04000204 = 0x00880808; /* WAITCNT */
    *(volatile u32 *)0x04000208 = 0x00000000; /* IME off */
    *(volatile u32 *)0x04000210 = 0x00000000; /* IE clear */
    *(volatile u32 *)0x04000218 = 0xFFFFFFFF; /* IF clear */
    *(volatile u32 *)0x04000300 = 0x00000000; /* POSTFLG */
}

void HwInit_ExMemCnt(void) {
    *(volatile u32 *)0x04000204 = 0xE8800000;
}
