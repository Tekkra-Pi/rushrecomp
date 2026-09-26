/* Collision grid functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* CollisionGrid_Init @ 0x02015400 (168 bytes)
 * Initializes collision grid.
 * Args: r0=grid, r1=cell_size, r2=width, r3=height */
void CollisionGrid_Init(void *grid, u32 cell_size, u32 width, u32 height) {
    u32 *data = (u32*)grid;
    data[0] = cell_size;
    data[1] = width;
    data[2] = height;
    
    /* Clear cells */
    u32 total_cells = width * height;
    u32 *cells = (u32*)(data + 4);
    for (u32 i = 0; i < total_cells; i++) {
        cells[i] = 0;
    }
}

/* CollisionGrid_Insert @ 0x020154a8 (232 bytes)
 * Inserts shape into grid.
 * Args: r0=grid, r1=shape_id, r2=bounds */
void CollisionGrid_Insert(void *grid, u32 shape_id, void *bounds) {
    u32 *data = (u32*)grid;
    u32 cell_size = data[0];
    u32 width = data[1];
    u32 height = data[2];
    u32 *cells = (u32*)(data + 4);
    
    s32 *b = (s32*)bounds;
    s32 min_x = b[0] / cell_size;
    s32 min_y = b[1] / cell_size;
    s32 max_x = b[2] / cell_size;
    s32 max_y = b[3] / cell_size;
    
    /* Clamp */
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= (s32)width) max_x = width - 1;
    if (max_y >= (s32)height) max_y = height - 1;
    
    /* Insert */
    for (s32 y = min_y; y <= max_y; y++) {
        for (s32 x = min_x; x <= max_x; x++) {
            cells[y * width + x] |= (1 << shape_id);
        }
    }
}

/* CollisionGrid_Remove @ 0x02015590 (296 bytes)
 * Removes shape from grid.
 * Args: r0=grid, r1=shape_id, r2=bounds */
void CollisionGrid_Remove(void *grid, u32 shape_id, void *bounds) {
    u32 *data = (u32*)grid;
    u32 cell_size = data[0];
    u32 width = data[1];
    u32 height = data[2];
    u32 *cells = (u32*)(data + 4);
    
    s32 *b = (s32*)bounds;
    s32 min_x = b[0] / cell_size;
    s32 min_y = b[1] / cell_size;
    s32 max_x = b[2] / cell_size;
    s32 max_y = b[3] / cell_size;
    
    /* Clamp */
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= (s32)width) max_x = width - 1;
    if (max_y >= (s32)height) max_y = height - 1;
    
    /* Remove */
    for (s32 y = min_y; y <= max_y; y++) {
        for (s32 x = min_x; x <= max_x; x++) {
            cells[y * width + x] &= ~(1 << shape_id);
        }
    }
}

/* CollisionGrid_Query @ 0x020156b8
 * Queries shapes in grid cell.
 * Args: r0=grid, r1=cell_x, r2=cell_y
 * Returns: shape mask */
u32 CollisionGrid_Query(void *grid, s32 cell_x, s32 cell_y) {
    u32 *data = (u32*)grid;
    u32 width = data[1];
    u32 *cells = (u32*)(data + 4);
    
    if (cell_x < 0 || cell_x >= (s32)width ||
        cell_y < 0 || cell_y >= (s32)data[2]) {
        return 0;
    }
    
    return cells[cell_y * width + cell_x];
}

/* CollisionGrid_QueryArea @ 0x02015760
 * Queries shapes in area.
 * Args: r0=grid, r1=bounds
 * Returns: combined shape mask */
u32 CollisionGrid_QueryArea(void *grid, void *bounds) {
    u32 *data = (u32*)grid;
    u32 cell_size = data[0];
    u32 width = data[1];
    u32 height = data[2];
    u32 *cells = (u32*)(data + 4);
    
    s32 *b = (s32*)bounds;
    s32 min_x = b[0] / cell_size;
    s32 min_y = b[1] / cell_size;
    s32 max_x = b[2] / cell_size;
    s32 max_y = b[3] / cell_size;
    
    /* Clamp */
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= (s32)width) max_x = width - 1;
    if (max_y >= (s32)height) max_y = height - 1;
    
    /* Combine masks */
    u32 mask = 0;
    for (s32 y = min_y; y <= max_y; y++) {
        for (s32 x = min_x; x <= max_x; x++) {
            mask |= cells[y * width + x];
        }
    }
    
    return mask;
}

/* CollisionGrid_GetCell @ 0x02015800
 * Returns cell coordinates for point.
 * Args: r0=grid, r1=x, r2=y, r3=out */
void CollisionGrid_GetCell(void *grid, s32 x, s32 y, void *out) {
    u32 *data = (u32*)grid;
    u32 cell_size = data[0];
    s32 *result = (s32*)out;
    
    result[0] = x / cell_size;
    result[1] = y / cell_size;
}

/* CollisionGrid_GetCellCount @ 0x02015860
 * Returns number of cells in grid.
 * Args: r0=grid
 * Returns: cell count */
u32 CollisionGrid_GetCellCount(void *grid) {
    u32 *data = (u32*)grid;
    return data[1] * data[2];
}

/* CollisionGrid_Clear @ 0x020158a0
 * Clears all cells in grid.
 * Args: r0=grid */
void CollisionGrid_Clear(void *grid) {
    u32 *data = (u32*)grid;
    u32 width = data[1];
    u32 height = data[2];
    u32 *cells = (u32*)(data + 4);
    
    u32 total_cells = width * height;
    for (u32 i = 0; i < total_cells; i++) {
        cells[i] = 0;
    }
}
