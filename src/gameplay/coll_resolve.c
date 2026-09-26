/* Gameplay level collision resolve functions. */
#include "nds_types.h"

void CollResolve_Init(void) { g_collision_pairs = 0; }

s32 CollResolve_Add(u32 type, void (*cb)(void*)) {
    if (g_collision_pairs >= 16) return -1;
    u32 i = g_collision_pairs;
    g_collresolves[i].type = type; g_collresolves[i].callback = cb;
    g_collresolves[i].active = 1; g_collision_pairs++; return i;
}

void CollResolve_Update(void) {
    u32 i;
    for (i = 0; i < g_collision_pairs; i++) {
        if (g_collresolves[i].active && g_collresolves[i].callback)
            g_collresolves[i].callback(&g_collresolves[i]);
    }
}

void CollResolve_Remove(u32 index) {
    if (index < g_collision_pairs) g_collresolves[index].active = 0;
}
u32 CollResolve_GetCount(void) { return g_collision_pairs; }
void CollResolve_Clear(void) { g_collision_pairs = 0; }
void CollResolve_Reset(void) { g_collision_pairs = 0; }
