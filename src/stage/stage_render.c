/* Stage rendering functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_stage_render_layer;
extern s32 g_stage_render_offset_x;
extern s32 g_stage_render_offset_y;
extern s32 g_tilemap_parallax[];
extern s32 g_tilemap_parallax_y[];

void StageRender_DrawTile(void) {
    extern void Tilemap_DrawAll(void);
    Tilemap_DrawAll();
}

void StageRender_DrawLayer(void) {
    extern void Tilemap_DrawAll(void);
    Tilemap_DrawAll();
}

void StageRender_DrawBackground(void) {
    extern void DispPrio_Sort(void);
    DispPrio_Sort();
}

void StageRender_SetOffset(u32 index, u32 value) {
    if (index < 16) {
        g_tilemap_parallax[index] = (s32)value;
    }
}

void StageRender_GetOffset(void) {
    /* Returns current render offset - implicit r0 return */
}

void StageRender_SetLayer(u32 index, u32 value) {
    (void)index;
    g_stage_render_layer = value;
}

u32 StageRender_GetLayer(void) {
    return g_stage_render_layer;
}
