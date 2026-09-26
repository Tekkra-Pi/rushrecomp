/* Gameplay level event system functions. */
#include "nds_types.h"


void EventSys_Init(void) { g_eventsys_count = 0; }

void EventSys_Trigger(u32 event_type, u32 param) {
    if (g_eventsys_count >= 16) return;
    u32 i = g_eventsys_count;
    g_eventsyss[i].event_type = event_type; g_eventsyss[i].param = param;
    g_eventsyss[i].processed = 0; g_eventsyss[i].active = 1;
    g_eventsys_count++;
}

void EventSys_Update(void) {
    u32 i;
    for (i = 0; i < g_eventsys_count; i++) {
        if (!g_eventsyss[i].active || g_eventsyss[i].processed) continue;
        switch (g_eventsyss[i].event_type) {
            case 0: /* spawn enemy */
                Enemy_Spawn(0, 0, g_eventsyss[i].param, 0);
                break;
            case 1: /* play sound */
                SoundEffect_Play(g_eventsyss[i].param, 0x40, 0x100);
                break;
            case 2: /* camera shake */
                CameraShake_Start(4, 30);
                break;
            case 3: /* level warp */
                LevelWarp_Start(g_eventsyss[i].param, 0, 0);
                break;
        }
        g_eventsyss[i].processed = 1;
    }
}

void EventSys_Clear(void) { g_eventsys_count = 0; }
void EventSys_Reset(void) { g_eventsys_count = 0; }
