/* Gameplay level enemy pattern functions. */

#include "nds_types.h"

static u32 s_enemypattern_count;

void EnemyPattern_Init(void) { s_enemypattern_count = 0; }

s32 EnemyPattern_Add(u32 type, u32 count) {
    if (s_enemypattern_count >= 16) return -1;
    u32 i = s_enemypattern_count;
    s_enemypattern_count++;
    return i;
}

void EnemyPattern_Update(void) { }

u32 EnemyPattern_GetCount(void) { return s_enemypattern_count; }

void EnemyPattern_Clear(void) { s_enemypattern_count = 0; }
void EnemyPattern_Reset(void) { s_enemypattern_count = 0; }
