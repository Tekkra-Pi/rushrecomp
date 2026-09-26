/* Overlay loader functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 187-203 */

#include "nds_types.h"

extern u32 g_overlayzone_count;
extern OverlayZoneEntry g_overlayzones[];
extern u32 g_zonemgr_zone;

s32 Overlay_Load(void) {
    /* Load overlay data for current zone */
    extern void Overlay_LoadFromROM(u32 zone_id);
    Overlay_LoadFromROM(g_zonemgr_zone);
    return 0;
}

s32 Overlay_Unload(void) {
    /* Unload current overlay */
    extern void Overlay_UnloadCurrent(void);
    Overlay_UnloadCurrent();
    return 0;
}
