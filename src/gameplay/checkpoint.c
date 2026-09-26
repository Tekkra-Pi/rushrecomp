/* Gameplay level checkpoint functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Sound_Play(u32 sfx);

#define CHECKPOINT_MAX 16

void Checkpoint_Init(void) { g_checkpoint_count = 0; }

s32 Checkpoint_Add(s32 x, s32 y, u32 param) {
    u32 i = g_checkpoint_count;
    g_respawnpts[i].x = x;
    g_respawnpts[i].y = y;
    g_respawnpts[i].active = 1;
    g_checkpoint_count++;
    return i;
}

void Checkpoint_Update(void) {
    u32 i;
    for (i = 0; i < g_checkpoint_count; i++) {
        if (!g_respawnpts[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_respawnpts[i].x;
        s32 dy = py - g_respawnpts[i].y;
        if (dx >= -0x400 && dx <= 0x400 && dy >= -0x400 && dy <= 0x400) {
            g_respawn_x = g_respawnpts[i].x;
            g_respawn_y = g_respawnpts[i].y;
            g_respawnpts[i].active = 0;
            Sound_Play(0x08);
        }
    }
}

void Checkpoint_Remove(u32 index) {
    if (index < g_checkpoint_count) g_respawnpts[index].active = 0;
}

u32 Checkpoint_GetCount(void) { return g_checkpoint_count; }

void Checkpoint_Clear(void) { g_checkpoint_count = 0; }
void Checkpoint_Reset(void) { g_checkpoint_count = 0; }
