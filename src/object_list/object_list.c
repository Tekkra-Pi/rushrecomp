/* Object-list system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 518-547 */

#include "nds_types.h"
#include "object_list.h"

/* Object-list bases at 0x2072c78 */
extern void* g_object_list_bases[2];

/* ObjectList_WalkAll @ 0x02036a0c (200 bytes)
 * Per-frame walker for object lists.
 * Iterates 32 slots, walks each chain, copies coords to compact table.
 * Args: r0=list_index (0 or 1) */
void ObjectList_WalkAll(int list_index) {
    ObjectListControl *control;
    CameraSpaceEntry *compact;
    u16 *slot_heads;
    u32 i;
    u32 obj_idx;
    
    /* Get list base */
    control = (ObjectListControl*)((u8*)g_object_list_bases[list_index] + 0x800);
    compact = (CameraSpaceEntry*)((u8*)g_object_list_bases[list_index] + 0x8a8);
    slot_heads = (u16*)((u8*)g_object_list_bases[list_index] + 0x1400 + 0xa8);
    
    /* Reset counters */
    control->object_count = 0;
    control->camera_space_a_count = 0;
    
    /* Clear camera-space entries */
    for (i = 0; i < 32; i++) {
        compact[i].coord_a = 0;
        compact[i].coord_b = 0;
        compact[i].coord_c = 0;
        compact[i].coord_d = 0;
    }
    
    /* Walk each slot chain */
    for (i = 0; i < 32; i++) {
        obj_idx = slot_heads[i];
        while (obj_idx != 0xffff) {
            ObjectEntry *entry = (ObjectEntry*)((u8*)g_object_list_bases[list_index] 
                                 + 0x1000 + (obj_idx * 8));
            
            /* Copy 6 bytes of coords to compact table */
            if (control->object_count < 0x80) {
                u16 *compact_slot = (u16*)&compact[control->object_count];
                compact_slot[0] = entry->data[0];
                compact_slot[1] = entry->data[1];
                compact_slot[2] = entry->data[2];
                control->object_count++;
            }
            
            /* Follow chain */
            obj_idx = entry->next_index;
        }
    }
}

/* ObjectList_Insert @ 0x02036bfc (64 bytes)
 * Inserts an object into the list at a given slot.
 * Args: r0=list_index, r1=slot_index (0-31)
 * Returns: 1 on success, 0 if full */
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
        return 0;  /* Full */
    }
    
    /* Get slot head pointer */
    slot_heads = (u16*)((u8*)g_object_list_bases[list_index] + 0x1400 + 0xa8);
    
    /* Insert at head of slot chain */
    /* This is simplified - actual implementation manages free list */
    control->object_count++;
    
    return 1;
}

/* ObjectList_CameraSpaceLookup @ 0x02036aec (260 bytes)
 * Looks up camera-space coordinates for an object.
 * Args: r0=list_index, r1=coords (4 words shifted <<12)
 * Returns: slot index, or 0xffff if full */
int ObjectList_CameraSpaceLookup(int list_index, s32 *coords) {
    ObjectListControl *control;
    CameraSpaceEntry *compact;
    u32 i;
    
    /* Get list base */
    control = (ObjectListControl*)((u8*)g_object_list_bases[list_index] + 0x800);
    compact = (CameraSpaceEntry*)((u8*)g_object_list_bases[list_index] + 0x8a8);
    
    /* Search for matching coordinates */
    for (i = 0; i < 32; i++) {
        if (compact[i].coord_a == (coords[0] >> 4) &&
            compact[i].coord_b == (coords[1] >> 4) &&
            compact[i].coord_c == (coords[2] >> 4) &&
            compact[i].coord_d == (coords[3] >> 4)) {
            return i;
        }
    }
    
    return 0xffff;  /* Not found / full */
}
