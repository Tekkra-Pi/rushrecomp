/* Event trigger and zone management functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "entity.h"

extern ActionTriggerEntry g_evttrigs[];
extern u32 g_actiontrig_count;
extern ExitTriggerEntry g_exittriggers[];
extern u32 g_exittrigger_count;
extern u32 g_entity_pool_size;
extern u32 g_entity_count;
extern u32 g_pool_used;

#define MAX_EVT_TRIGS 32
#define MAX_ENTITIES 128

s32 EventTrigger_Check(void) {
    u32 i;
    for (i = 0; i < g_actiontrig_count; i++) {
        if (g_evttrigs[i].active && !g_evttrigs[i].triggered) {
            return (s32)i;
        }
    }
    return -1;
}

void EventTrigger_Fire(void) {
    u32 i;
    for (i = 0; i < g_actiontrig_count; i++) {
        if (g_evttrigs[i].active && !g_evttrigs[i].triggered) {
            extern void EventSys_Trigger(u32, u32);
            EventSys_Trigger(g_evttrigs[i].event_type, g_evttrigs[i].param);
            g_evttrigs[i].triggered = 1;
        }
    }
}

u32 EntityList_GetCount(void) {
    return g_entity_count;
}

void EntityList_SortByPriority(void) {
    /* Sort entity list by priority/type field */
    extern EntityContainer* Entity_GetHead(u32);
    EntityContainer *head = Entity_GetHead(0);
    EntityContainer *a, *b;
    if (!head) return;
    /* Simple bubble sort by type (priority in low 3 bits) */
    for (a = head; a; a = a->next) {
        for (b = a->next; b; b = b->next) {
            if ((a->type & 7) > (b->type & 7)) {
                u32 tmp_type = a->type;
                u32 tmp_flags = a->flags;
                void *tmp_data = a->data;
                a->type = b->type;
                a->flags = b->flags;
                a->data = b->data;
                b->type = tmp_type;
                b->flags = tmp_flags;
                b->data = tmp_data;
            }
        }
    }
}

void EntityList_RemoveAll(void) {
    extern void Entity_RemoveAll(void);
    Entity_RemoveAll();
    g_entity_count = 0;
    g_pool_used = 0;
}
