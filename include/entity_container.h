/* EntityContainer struct - 0x1C bytes, linked list node for entity system.
 * Based on member access analysis of entity/*.c and player/*.c files. */

#ifndef ENTITY_CONTAINER_H
#define ENTITY_CONTAINER_H

#include "nds_types.h"

typedef struct EntityContainer {
    /* +0x00 */ struct EntityContainer *next;
    /* +0x04 */ struct EntityContainer *prev;
    /* +0x08 */ u32 type;
    /* +0x0C */ u32 flags;
    /* +0x10 */ u32 size;
    /* +0x14 */ void *data;
    /* +0x18 */ void (*update)(struct EntityContainer *);
    /* +0x1C */ void (*destroy)(struct EntityContainer *);
    /* +0x20 */ u32 marker;
} EntityContainer;

/* EntityContainer flags */
#define EF_ACTIVE         (1 << 0)
#define EF_VISIBLE        (1 << 1)
#define EF_DESTROYED      (1 << 2)
#define EF_PLAYER         (1 << 3)
#define EF_ENEMY          (1 << 4)
#define EF_OBJECT         (1 << 5)

#endif /* ENTITY_CONTAINER_H */