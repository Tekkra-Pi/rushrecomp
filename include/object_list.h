#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H

#include "nds_types.h"

/* Object-list control header (at base + 0x800)
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 524-534 */
typedef struct {
    u32 _pad00[0x27];           /* +0x00: unknown */
    u16 object_count;           /* +0x9c: object count (capacity 0x80) */
    u16 camera_space_a_count;   /* +0x9e: camera-space A count */
    u32 _pada0[2];              /* +0xa0: unknown */
    u16 camera_space_b_count;   /* +0x8a0: camera-space B count (sum must be < 0x20) */
    u16 _pad8a2;                /* +0x8a2: padding */
    u32 _pad8a4[2];             /* +0x8a4: unknown */
} ObjectListControl;

/* 32-byte camera-space entry (at base + 0x8a8 + i*0x20)
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 528-529 */
typedef struct {
    u32 _pad00;                 /* +0x00: unknown */
    s16 coord_a;                /* +0x06: signed halfword coord A */
    u32 _pad08;                 /* +0x08: unknown */
    s16 coord_b;                /* +0x0e: signed halfword coord B */
    u32 _pad10;                 /* +0x10: unknown */
    s16 coord_c;                /* +0x16: signed halfword coord C */
    u32 _pad18;                 /* +0x18: unknown */
    s16 coord_d;                /* +0x1e: signed halfword coord D */
} CameraSpaceEntry;

/* 8-byte object entry (at base + 0x1000 + i*8)
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 530-531 */
typedef struct {
    u16 data[3];                /* +0x00: object data */
    u16 next_index;             /* +0x06: next index in chain (0xffff = end) */
} ObjectEntry;

/* Per-slot list heads (at base + 0x1400 + slot*2 + 0xa8, 32 slots)
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 532 */
typedef struct {
    u16 slot_heads[32];         /* +0x00: per-slot list heads */
} SlotListHeads;

/* Object-list base table at 0x2072c78
 * Evidence: CONFIRMED-STATIC from HANDOFF.md line 423 */
extern void* g_object_list_bases[2];

/* Object-list functions */
void ObjectList_WalkAll(int list_index);
int ObjectList_Insert(int list_index, int slot_index);
int ObjectList_CameraSpaceLookup(int list_index, s32* coords);

#endif /* OBJECT_LIST_H */
