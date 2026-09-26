/* Gameplay event helper functions. */
#include "nds_types.h"

void EventHelper_Init(void) { g_eventsys_count = 0; }
void EventHelper_Trigger(void) {
    u32 i;
    for (i = 0; i < g_eventsys_count; i++)
        if (g_eventsyss[i].active) g_eventsyss[i].processed = 1;
}
void EventHelper_Update(void) {
    u32 i;
    for (i = 0; i < g_eventsys_count; i++)
        if (g_eventsyss[i].active && g_eventsyss[i].processed) g_eventsyss[i].active = 0;
}
void EventHelper_Fire(void) { EventHelper_Trigger(); }
void EventHelper_Clear(void) { g_eventsys_count = 0; }
u32 EventHelper_GetCount(void) { return g_eventsys_count; }
s32 EventHelper_IsActive(u32 index) {
    return index < g_eventsys_count ? g_eventsyss[index].active : 0;
}
void EventHelper_SetActive(u32 index, u32 value) {
    if (index < g_eventsys_count) g_eventsyss[index].active = value;
}
s32 EventHelper_Wait(void) {
    u32 i;
    for (i = 0; i < g_eventsys_count; i++)
        if (g_eventsyss[i].active && !g_eventsyss[i].processed) return 1;
    return 0;
}
s32 EventHelper_WaitForTime(void) {
    u32 i;
    for (i = 0; i < g_eventsys_count; i++)
        if (g_eventsyss[i].active) return 1;
    return 0;
}
void EventHelper_Reset(void) { g_eventsys_count = 0; }
