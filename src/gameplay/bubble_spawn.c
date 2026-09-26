/* Gameplay level bubble spawn functions. */

#include "nds_types.h"

#define BUBBLESPAWN_MAX 16

void BubbleSpawn_Init(void) { }

void BubbleSpawn_Update(void) {
    u32 i;
    for (i = 0; i < BUBBLESPAWN_MAX; i++) {
        if (!g_bubbles[i].active) continue;
        g_bubbles[i].y -= 0x40;
        g_bubbles[i].timer++;
        if (g_bubbles[i].timer >= 60) {
            g_bubbles[i].active = 0;
        }
    }
}

void BubbleSpawn_UpdateAll(void) {
    BubbleSpawn_Update();
}

void BubbleSpawn_Clear(void) {
    u32 i;
    for (i = 0; i < BUBBLESPAWN_MAX; i++) g_bubbles[i].active = 0;
}

void BubbleSpawn_Reset(void) {
    BubbleSpawn_Clear();
}
