/* Stage loading functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_stage_load_progress;
extern u32 g_stageload_state;
extern u32 g_stage_tileset_id;
extern s32 g_stage_width;
extern s32 g_stage_height;
extern u32 g_stage_data_ptr;

void StageLoad_DMAStageData(void) {
    extern void DMA_LoadStageData(u32 zone, u32 act);
    extern u32 g_current_zone, g_current_act;
    DMA_LoadStageData(g_current_zone, g_current_act);
    g_stageload_state = 1;
}

void StageLoad_ProcessData(void) {
    if (g_stageload_state == 1) {
        extern void Tilemap_Init(void);
        Tilemap_Init();
        g_stageload_state = 2;
    }
    g_stage_load_progress = g_stageload_state == 2 ? 100 : 0;
}

s32 StageLoad_IsComplete(u32 index) {
    (void)index;
    return g_stageload_state == 2 ? 1 : 0;
}

u32 StageLoad_GetProgress(void) {
    return g_stage_load_progress;
}

u32 StageLoad_GetWidth(void) {
    return (u32)g_stage_width;
}

u32 StageLoad_GetHeight(void) {
    return (u32)g_stage_height;
}

u32 StageLoad_GetTileset(void) {
    return g_stage_tileset_id;
}
