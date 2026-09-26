/* Gameplay level collision dispatch functions. */
#include "nds_types.h"

void CollDispatch_Init(void) { g_collision_pairs = 0; }

s32 CollDispatch_Add(u32 ta, u32 tb, void (*cb)(void*)) {
    if (g_collision_pairs >= 16) return -1;
    u32 i = g_collision_pairs;
    g_colldispatchs[i].type_a = ta; g_colldispatchs[i].type_b = tb;
    g_colldispatchs[i].callback = cb; g_colldispatchs[i].active = 1;
    g_collision_pairs++; return i;
}

void CollDispatch_Update(void) {
    u32 i;
    for (i = 0; i < g_collision_pairs; i++) {
        if (g_colldispatchs[i].active && g_colldispatchs[i].callback)
            g_colldispatchs[i].callback(&g_colldispatchs[i]);
    }
}

void CollDispatch_Remove(u32 index) {
    if (index < g_collision_pairs) g_colldispatchs[index].active = 0;
}

u32 CollDispatch_GetCount(void) { return g_collision_pairs; }
void CollDispatch_Clear(void) { g_collision_pairs = 0; }
void CollDispatch_Reset(void) { g_collision_pairs = 0; }
