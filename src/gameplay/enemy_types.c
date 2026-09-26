/* Gameplay level enemy types functions. */

#include "nds_types.h"

void EnemyTypes_Init(void) { }

u32 EnemyTypes_GetHP(u32 type) {
    switch (type) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        default: return 1;
    }
}

u32 EnemyTypes_GetSpeed(u32 type) {
    switch (type) {
        case 0: return 0x40;
        case 1: return 0x60;
        case 2: return 0x80;
        default: return 0x40;
    }
}

u32 EnemyTypes_GetScore(u32 type) {
    switch (type) {
        case 0: return 100;
        case 1: return 200;
        case 2: return 300;
        default: return 100;
    }
}

void EnemyTypes_Clear(void) { }
void EnemyTypes_Reset(void) { }
