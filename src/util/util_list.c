/* Utility linked list functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* List_Init @ 0x0200dc00 (148 bytes)
 * Initializes linked list.
 * Args: r0=list */
void List_Init(void *list) {
    u32 *data = (u32*)list;
    data[0] = 0;  /* head */
    data[1] = 0;  /* tail */
    data[2] = 0;  /* count */
}

/* List_AddFront @ 0x0200dc94 (216 bytes)
 * Adds item to front of list.
 * Args: r0=list, r1=item */
void List_AddFront(void *list, void *item) {
    u32 *data = (u32*)list;
    u32 *node = (u32*)item;
    
    node[0] = data[0];  /* next = old head */
    node[1] = 0;        /* prev = NULL */
    
    if (data[0]) {
        u32 *old_head = (u32*)data[0];
        old_head[1] = (u32)item;
    }
    
    data[0] = (u32)item;
    
    if (!data[1]) {
        data[1] = (u32)item;  /* Update tail if empty */
    }
    
    data[2]++;  /* Increment count */
}

/* List_AddBack @ 0x0200dd70 (264 bytes)
 * Adds item to back of list.
 * Args: r0=list, r1=item */
void List_AddBack(void *list, void *item) {
    u32 *data = (u32*)list;
    u32 *node = (u32*)item;
    
    node[0] = 0;        /* next = NULL */
    node[1] = data[1];  /* prev = old tail */
    
    if (data[1]) {
        u32 *old_tail = (u32*)data[1];
        old_tail[0] = (u32)item;
    }
    
    data[1] = (u32)item;
    
    if (!data[0]) {
        data[0] = (u32)item;  /* Update head if empty */
    }
    
    data[2]++;  /* Increment count */
}

/* List_Remove @ 0x0200de78
 * Removes item from list.
 * Args: r0=list, r1=item */
void List_Remove(void *list, void *item) {
    u32 *data = (u32*)list;
    u32 *node = (u32*)item;
    
    /* Update prev node */
    if (node[1]) {
        u32 *prev = (u32*)node[1];
        prev[0] = node[0];
    } else {
        data[0] = node[0];  /* Update head */
    }
    
    /* Update next node */
    if (node[0]) {
        u32 *next = (u32*)node[0];
        next[1] = node[1];
    } else {
        data[1] = node[1];  /* Update tail */
    }
    
    data[2]--;  /* Decrement count */
}

/* List_GetFirst @ 0x0200df20
 * Returns first item in list.
 * Args: r0=list
 * Returns: first item or NULL */
void* List_GetFirst(void *list) {
    u32 *data = (u32*)list;
    return (void*)data[0];
}

/* List_GetLast @ 0x0200df40
 * Returns last item in list.
 * Args: r0=list
 * Returns: last item or NULL */
void* List_GetLast(void *list) {
    u32 *data = (u32*)list;
    return (void*)data[1];
}

/* List_GetCount @ 0x0200df60
 * Returns number of items in list.
 * Args: r0=list
 * Returns: count */
u32 List_GetCount(void *list) {
    u32 *data = (u32*)list;
    return data[2];
}

/* List_IsEmpty @ 0x0200df80
 * Returns whether list is empty.
 * Args: r0=list
 * Returns: 1 if empty, 0 otherwise */
s32 List_IsEmpty(void *list) {
    u32 *data = (u32*)list;
    return data[2] == 0;
}
