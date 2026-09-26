#include "nds_types.h"
static u32 s_count;
void OverlayZone_Init(void) { s_count = 0; g_overlayzone_count = 0; }
s32 OverlayZone_Add(u32 zone_id) {
    if (s_count >= 8) return -1;
    s_count++; g_overlayzone_count = s_count; return s_count - 1;
}
void OverlayZone_Update(void) { }
void OverlayZone_Remove(u32 index) { (void)index; s_count = 0; g_overlayzone_count = 0; }
u32 OverlayZone_GetCount(void) { return s_count; }
void OverlayZone_Clear(void) { s_count = 0; g_overlayzone_count = 0; }
void OverlayZone_Reset(void) { OverlayZone_Clear(); }
