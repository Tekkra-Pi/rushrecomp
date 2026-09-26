/* System event functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern EventEntry g_events[];
extern u32 g_eventsys_count;
extern EventSysEntry g_eventsyss[];

#define MAX_EVENTS 32

void Event_Init(void) {
    u32 i;
    g_eventsys_count = 0;
    for (i = 0; i < MAX_EVENTS; i++) {
        g_events[i].active = 0;
        g_eventsyss[i].active = 0;
    }
}

void Event_Unregister(void) {
    g_eventsys_count = 0;
}

void Event_Fire(void) {
    u32 i;
    for (i = 0; i < g_eventsys_count; i++) {
        if (g_eventsyss[i].active && !g_eventsyss[i].processed) {
            g_eventsyss[i].processed = 1;
        }
    }
}

void Event_FireWithData(void) {
    Event_Fire();
}

u32 Event_GetCount(void) {
    return g_eventsys_count;
}

void Event_Clear(void) {
    u32 i;
    for (i = 0; i < g_eventsys_count; i++) {
        g_eventsyss[i].processed = 0;
    }
}
