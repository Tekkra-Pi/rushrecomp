/* Early ARM9 hardware init - sound. */
#include "nds_types.h"

void HwInit_Sound(void) {
    /* Disable all sound */
    *(volatile u16 *)0x04000504 = 0; /* SOUND_MASTER_ENABLE */
    volatile u16 *ch = (volatile u16 *)0x04000400;
    u32 i;
    for (i = 0; i < 16; i++) {
        ch[i * 4] = 0;      /* SxCNT */
        ch[i * 4 + 1] = 0;  /* SxSAD */
        ch[i * 4 + 2] = 0;  /* SxTMR */
        ch[i * 4 + 3] = 0;  /* SxPNT */
    }
    /* Master volume */
    *(volatile u16 *)0x04000500 = 0;
}

void HwInit_SoundCapture(void) {
    *(volatile u16 *)0x04000508 = 0; /* SOUNDCNT_X */
    *(volatile u32 *)0x04000510 = 0; /* SNDCAP */
}
