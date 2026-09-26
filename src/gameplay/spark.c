/* Gameplay level spark functions. */
#include "nds_types.h"

#define SPARK_MAX 16

void Spark_Init(void) { }

s32 Spark_Spawn(s32 x, s32 y) {
    u32 i;
    for (i = 0; i < SPARK_MAX; i++) {
        if (g_sparks[i].active) continue;
        g_sparks[i].x = x;
        g_sparks[i].y = y;
        g_sparks[i].vel_x = (i & 1) ? 0x100 : -0x100;
        g_sparks[i].vel_y = -0x100 + (i * 0x20);
        g_sparks[i].timer = 0;
        g_sparks[i].active = 1;
        return i;
    }
    return -1;
}

void Spark_Update(void) {
    u32 i;
    for (i = 0; i < SPARK_MAX; i++) {
        if (!g_sparks[i].active) continue;
        g_sparks[i].x += g_sparks[i].vel_x;
        g_sparks[i].y += g_sparks[i].vel_y;
        g_sparks[i].vel_y += 0x20;
        g_sparks[i].timer++;
        if (g_sparks[i].timer >= 15) g_sparks[i].active = 0;
    }
}

void Spark_Draw(void) { }

void Spark_Remove(u32 index) {
    if (index < SPARK_MAX) g_sparks[index].active = 0;
}

u32 Spark_GetCount(void) { return SPARK_MAX; }

void Spark_Clear(void) {
    u32 i;
    for (i = 0; i < SPARK_MAX; i++) g_sparks[i].active = 0;
}

void Spark_Reset(void) { Spark_Clear(); }
