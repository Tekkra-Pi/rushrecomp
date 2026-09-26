/* Early ARM9 hardware init - IPC/mailbox. */

#include "nds_types.h"

void HwInit_IPC(void) {
    REG_IPC_SYNC = 0;
    REG_IME = 0;
    REG_IE = 0;
    REG_IF = 0;
    /* Initialize IPC FIFO */
    *(vu32*)0x04100000 = 0x00000000;
}

void HwInit_Mailbox(void) {
    /* Initialize ARM7/ARM9 mailbox */
    *(vu32*)0x04000180 = 0;
    *(vu32*)0x04000184 = 0;
}
