#include "nds_types.h"
void ObjRender_Init(void) { g_objrender_count = 0; }
void ObjRender_Add(s32 x, s32 y, u32 tile, u32 attr) {
    (void)x; (void)y; (void)tile; (void)attr;
    if (g_objrender_count < 256) g_objrender_count++;
}
void ObjRender_Update(void) { }
void ObjRender_Draw(void) { }
void ObjRender_Remove(u32 index) { (void)index; }
u32 ObjRender_GetCount(void) { return g_objrender_count; }
void ObjRender_Clear(void) { g_objrender_count = 0; }
void ObjRender_Reset(void) { g_objrender_count = 0; }
