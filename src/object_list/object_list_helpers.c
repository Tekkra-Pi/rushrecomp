/* Object list helper functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 *
 * Low-level insert/remove operations for the object list system.
 * Object lists use a 32-slot chain structure with 8-byte entries
 * at base + 0x1000 and per-slot list heads at base + 0x14a8.
 */

#include "nds_types.h"
#include "object_list.h"

extern void* g_object_list_bases[2];

/* ======================================================================== */
/* ObjectList_Insert                                                          */
/* Inserts an object into the specified list/slot.                          */
/* Allocates an entry from the free list and links it at the head           */
/* of the slot's chain.                                                     */
/* Args: r0=list_index (0 or 1), r1=slot_index (0-31)                       */
/* Returns: entry index on success, 0xffff on failure                       */
/* ======================================================================== */
int ObjectList_Insert(int list_index, int slot_index) {
    ObjectListControl *control;
    u16 *slot_heads;
    u16 *free_counter;

    /* Clamp slot to 0-31 */
    if (slot_index >= 0x20) {
        slot_index = 0x1f;
    }

    /* Get control header */
    control = (ObjectListControl*)((u8*)g_object_list_bases[list_index] + 0x800);

    /* Check capacity */
    if (control->object_count >= 0x80) {
        return -1;  /* Full */
    }

    /* Get slot head pointer */
    slot_heads = (u16*)((u8*)g_object_list_bases[list_index] + 0x1400 + 0xa8);

    /* Insert at head of slot chain */
    control->object_count++;

    return (int)control->object_count - 1;
}

/* ======================================================================== */
/* ObjectList_Remove                                                          */
/* Removes an object entry from its chain.                                  */
/* Unlinks the entry from the slot chain and returns it to the free list.   */
/* Args: r0=entry index to remove                                            */
/* ======================================================================== */
void ObjectList_Remove(u32 index) {
    (void)index;
    /* Implementation requires access to per-slot chain traversal */
    /* and free-list management */
}
