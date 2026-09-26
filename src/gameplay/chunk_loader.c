/* Gameplay level chunk loader functions. */
#include "nds_types.h"

void ChunkLoader_Init(void) { g_chunkload_radius = 4; }
void ChunkLoader_SetRadius(u32 index, u32 value) { (void)index; g_chunkload_radius = value; }
void ChunkLoader_Update(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    g_chunkload_x = px; g_chunkload_y = py;
}
void ChunkLoader_LoadNearby(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    s32 cx = px / 512, cy = py / 512, r = (s32)g_chunkload_radius;
    s32 dx, dy;
    for (dy = -r; dy <= r; dy++)
        for (dx = -r; dx <= r; dx++)
            StageChunk_Load(cx + dx, cy + dy);
}
s32 ChunkLoader_GetChunkX(void) { return g_chunkload_x / 512; }
s32 ChunkLoader_GetChunkY(void) { return g_chunkload_y / 512; }
u32 ChunkLoader_GetRadius(void) { return g_chunkload_radius; }
void ChunkLoader_Reset(void) { g_chunkload_radius = 4; }
