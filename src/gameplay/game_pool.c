/* Gameplay pool functions. */
#include "nds_types.h"

static u32 s_used;

void Pool_Init(void) { s_used = 0; }
void Pool_Free(void) { s_used = 0; }
u32 Pool_GetSize(u32 index) { (void)index; return g_pool_total_size; }
u32 Pool_GetUsed(void) { return s_used; }
u32 Pool_GetFree(void) { return g_pool_total_size - s_used; }
void Pool_Clear(void) { s_used = 0; }
void Pool_AddBlock(void) { }
