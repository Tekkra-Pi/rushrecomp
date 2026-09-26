#include "nds_types.h"
void ObjListMgr_Init(void) { g_objlistmgr_count = 0; }
void ObjListMgr_Add(u32 list_type) {
    if (g_objlistmgr_count >= 8) return;
    g_objlistmgrs[g_objlistmgr_count].list_type = list_type;
    g_objlistmgrs[g_objlistmgr_count].count = 0;
    g_objlistmgrs[g_objlistmgr_count].active = 1;
    g_objlistmgr_count++;
}
void ObjListMgr_Update(void) { }
void ObjListMgr_Remove(u32 index) { if (index < g_objlistmgr_count) g_objlistmgrs[index].active = 0; }
u32 ObjListMgr_GetCount(void) { return g_objlistmgr_count; }
void ObjListMgr_Clear(void) { g_objlistmgr_count = 0; }
void ObjListMgr_Reset(void) { g_objlistmgr_count = 0; }
