/* Gameplay level grind rail actor functions. */
#include "nds_types.h"

void GrindActor_Init(void) { g_grindactor_count = 0; }

s32 GrindActor_Add(const s32 *points, u32 num_points, u32 speed) {
    if (g_grindactor_count >= 8) return -1;
    u32 i = g_grindactor_count;
    g_grindactors[i].points = points; g_grindactors[i].num_points = num_points;
    g_grindactors[i].speed = speed; g_grindactors[i].active = 1;
    g_grindactor_count++; return i;
}

void GrindActor_Remove(u32 i) { if (i < g_grindactor_count) g_grindactor_count--; }

s32 GrindActor_GetStartX(u32 i) { return (i < g_grindactor_count) ? g_grindactors[i].points[0] * 256 : 0; }
s32 GrindActor_GetStartY(u32 i) { return (i < g_grindactor_count) ? g_grindactors[i].points[1] * 256 : 0; }
s32 GrindActor_GetEndX(u32 i) {
    if (i >= g_grindactor_count) return 0;
    u32 n = g_grindactors[i].num_points;
    return g_grindactors[i].points[(n - 1) * 2] * 256;
}
s32 GrindActor_GetEndY(u32 i) {
    if (i >= g_grindactor_count) return 0;
    u32 n = g_grindactors[i].num_points;
    return g_grindactors[i].points[(n - 1) * 2 + 1] * 256;
}

s32 GrindActor_GetSpeed(u32 i) { return (i < g_grindactor_count) ? g_grindactors[i].speed : 0; }
u32 GrindActor_GetNumPoints(u32 i) { return (i < g_grindactor_count) ? g_grindactors[i].num_points : 0; }
u32 GrindActor_GetCount(void) { return g_grindactor_count; }
s32 GrindActor_IsActive(u32 i) { return (i < g_grindactor_count) ? g_grindactors[i].active : 0; }
void GrindActor_Clear(void) { g_grindactor_count = 0; }
void GrindActor_Reset(void) { g_grindactor_count = 0; }
