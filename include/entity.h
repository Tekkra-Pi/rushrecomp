#ifndef ENTITY_H
#define ENTITY_H

#include "nds_types.h"

/* Entity container struct (0x1c bytes) at 0x22b4574 linked list
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 424-435 */
typedef struct EntityContainer {
    struct EntityContainer* prev;   /* +0x00: previous link / list node */
    struct EntityContainer* next;   /* +0x04: next link (walker: r6 = [r6,4]) */
    void (*update)(struct EntityContainer*); /* +0x08: update fn ptr */
    void (*destroy)(struct EntityContainer*); /* +0x0c: destroy fn ptr */
    void* data;                     /* +0x10: DATA ptr (e.g., 0x134 player / 0x77c pause) */
    u16 flags;                      /* +0x14: halfword flags (bit0 = skip in walk) */
    u16 type;                       /* +0x16: halfword type (low 3 bits = phase/priority, bit4 = always-update) */
    u16 size;                       /* +0x18: halfword size/cap */
    u16 marker;                     /* +0x1a: halfword marker (0xffff default) */
} EntityContainer;

/* Entity list globals */
#define ENTITY_LIST_HEAD    0x22b4574
#define ENTITY_LIST_COUNT   0x22b4578  /* count at +0, cap 0x100 */
#define ENTITY_FREE_TABLE   0x22b458c

/* Special container types */
#define ENTITY_TYPE_SPECIAL_20    0x0020
#define ENTITY_TYPE_SPECIAL_EFFF  0xEFFF
#define ENTITY_TYPE_SPECIAL_F000  0xF000

/* Entity flags */
#define ENTITY_FLAG_SKIP_WALK     (1 << 0)
#define ENTITY_FLAG_ALWAYS_UPDATE (1 << 4)

/* Entity type field masks */
#define ENTITY_TYPE_PHASE_MASK    0x07  /* low 3 bits = priority */
#define ENTITY_TYPE_ALWAYS_UPDATE (1 << 4)

/* System flags at 0x2087cb8 */
#define SYS_FLAG_UPDATE_FREEZE    (1 << 31)
#define SYS_FLAG_STATE_CHANGE     (1 << 8)

/* External globals */
extern EntityContainer g_entity_list_head;

/* Entity functions */
EntityContainer* Entity_AllocContainer(void (*update_cb)(EntityContainer*),
                                       void (*destroy_cb)(EntityContainer*),
                                       u16 type, u16 marker, u16 data_size);
void Entity_WalkAll(EntityContainer* start, EntityContainer* terminator);
void Entity_Remove(EntityContainer* entity);

#endif /* ENTITY_H */
