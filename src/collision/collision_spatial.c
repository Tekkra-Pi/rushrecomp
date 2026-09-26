/* Collision spatial functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* External globals */
extern u32 g_spatial_grid[];
extern u32 g_spatial_grid_size;
extern SpatialShapeEntry g_spatial_shapes[];

/* Spatial grid constants */
#define SPATIAL_GRID_MAX 256
#define SPATIAL_SHAPES_MAX 64
#define SPATIAL_CELL_SIZE 64

/* Forward declarations */
void CollisionSpatial_RebuildGrid(void);
void CollisionSpatial_InsertIntoGrid(u32 shape_index);

/* Spatial grid state */
static u32 spatial_grid_width = 0;
static u32 spatial_grid_height = 0;
static u32 spatial_shape_count = 0;

/* CollisionSpatial_Init
 * Initializes the spatial grid system.
 * Args: r0=width, r1=height */
void CollisionSpatial_Init(u32 width, u32 height) {
    spatial_grid_width = width;
    spatial_grid_height = height;
    spatial_shape_count = 0;

    /* Clear grid cells */
    u32 total = width * height;
    if (total > SPATIAL_GRID_MAX) total = SPATIAL_GRID_MAX;

    for (u32 i = 0; i < total; i++) {
        g_spatial_grid[i] = 0;
    }

    g_spatial_grid_size = total;
}

/* CollisionSpatial_Add
 * Adds a shape to the spatial system.
 * Args: r0=x, r1=y, r2=param
 * Returns: shape index, or -1 if full */
s32 CollisionSpatial_Add(s32 x, s32 y, u32 param) {
    if (spatial_shape_count >= SPATIAL_SHAPES_MAX) return -1;

    u32 idx = spatial_shape_count;
    g_spatial_shapes[idx].x = x;
    g_spatial_shapes[idx].y = y;
    g_spatial_shapes[idx].w = (param >> 16) & 0xFFFF;
    g_spatial_shapes[idx].h = param & 0xFFFF;
    g_spatial_shapes[idx].type = 0;
    g_spatial_shapes[idx].active = 1;

    spatial_shape_count++;
    return (s32)idx;
}

/* CollisionSpatial_Remove
 * Removes a shape from the spatial system.
 * Args: r0=index */
void CollisionSpatial_Remove(u32 index) {
    if (index < spatial_shape_count) {
        g_spatial_shapes[index].active = 0;
    }
}

/* CollisionSpatial_Update
 * Updates all shapes in the spatial grid.
 * Args: none */
void CollisionSpatial_Update(void) {
    /* Rebuild grid from active shapes */
    CollisionSpatial_RebuildGrid();
}

/* CollisionSpatial_RebuildGrid
 * Rebuilds the spatial grid from all active shapes.
 * Args: none */
void CollisionSpatial_RebuildGrid(void) {
    /* Clear grid */
    u32 total = spatial_grid_width * spatial_grid_height;
    if (total > SPATIAL_GRID_MAX) total = SPATIAL_GRID_MAX;

    for (u32 i = 0; i < total; i++) {
        g_spatial_grid[i] = 0;
    }

    /* Insert all active shapes */
    for (u32 i = 0; i < spatial_shape_count; i++) {
        if (!g_spatial_shapes[i].active) continue;

        CollisionSpatial_InsertIntoGrid(i);
    }
}

/* CollisionSpatial_InsertIntoGrid
 * Inserts a shape into the spatial grid.
 * Args: r0=shape_index */
void CollisionSpatial_InsertIntoGrid(u32 shape_index) {
    if (shape_index >= spatial_shape_count) return;

    SpatialShapeEntry *shape = &g_spatial_shapes[shape_index];
    if (!shape->active) return;

    /* Calculate grid cells that this shape overlaps */
    s32 min_x = shape->x / SPATIAL_CELL_SIZE;
    s32 min_y = shape->y / SPATIAL_CELL_SIZE;
    s32 max_x = (shape->x + shape->w - 1) / SPATIAL_CELL_SIZE;
    s32 max_y = (shape->y + shape->h - 1) / SPATIAL_CELL_SIZE;

    /* Clamp to grid bounds */
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= (s32)spatial_grid_width) max_x = spatial_grid_width - 1;
    if (max_y >= (s32)spatial_grid_height) max_y = spatial_grid_height - 1;

    /* Set bit for this shape in each overlapping cell */
    for (s32 y = min_y; y <= max_y; y++) {
        for (s32 x = min_x; x <= max_x; x++) {
            u32 cell_idx = y * spatial_grid_width + x;
            if (cell_idx < SPATIAL_GRID_MAX) {
                g_spatial_grid[cell_idx] |= (1 << shape_index);
            }
        }
    }
}

/* CollisionSpatial_Query
 * Queries shapes at a grid cell.
 * Args: r0=cell_x, r1=cell_y
 * Returns: bitmask of shapes in cell */
u32 CollisionSpatial_Query(s32 cell_x, s32 cell_y) {
    if (cell_x < 0 || cell_x >= (s32)spatial_grid_width ||
        cell_y < 0 || cell_y >= (s32)spatial_grid_height) {
        return 0;
    }

    u32 cell_idx = cell_y * spatial_grid_width + cell_x;
    if (cell_idx >= SPATIAL_GRID_MAX) return 0;

    return g_spatial_grid[cell_idx];
}

/* CollisionSpatial_GetCount
 * Returns the number of active spatial shapes.
 * Args: none
 * Returns: shape count */
u32 CollisionSpatial_GetCount(void) {
    return spatial_shape_count;
}

/* CollisionSpatial_Clear
 * Clears all shapes and the grid.
 * Args: none */
void CollisionSpatial_Clear(void) {
    spatial_shape_count = 0;

    u32 total = spatial_grid_width * spatial_grid_height;
    if (total > SPATIAL_GRID_MAX) total = SPATIAL_GRID_MAX;

    for (u32 i = 0; i < total; i++) {
        g_spatial_grid[i] = 0;
    }
}
