/* Early ARM9 hardware init - cart interface. */

#include "nds_types.h"

void HwInit_Cart(void) {
    /* Configure cartridge ROM access timing */
    *(vu32*)0x04000204 = 0x00180018;
    *(vu32*)0x04000208 = 0x00000000;
}

void HwInit_Card(void) {
    /* Initialize card (NAND/EEPROM) interface */
    *(vu32*)0x04000208 = 0x00000000;
}
