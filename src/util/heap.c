/* Heap/memory allocation functions.
 * Evidence: INFERRED from re-analysis.md and entity system usage */

#include "nds_types.h"

/* Heap memory block header (magic: 0x2467531 / 0xcccccccc) */
typedef struct HeapBlock {
    struct HeapBlock *next;     /* +0x00: next block */
    u32 size;                   /* +0x04: block size (including header) */
    u32 magic;                  /* +0x08: magic number (0x2467531 allocated, 0xcccccccc freed) */
    u8  data[];                 /* +0x0c: data starts here */
} HeapBlock;

/* Heap base pointers (from re-analysis) */
extern HeapBlock* g_heap_base;
extern HeapBlock* g_heap_end;

/* heap_alloc @ 0x02034a40
 * Allocates memory from the heap.
 * Args: r0=size, r1=magic
 * Returns: pointer to allocated memory, or NULL on failure */
void* heap_alloc(u32 size, u32 magic) {
    HeapBlock *block;
    HeapBlock *best = NULL;
    u32 best_size = 0xFFFFFFFF;
    
    /* Search for free block that fits */
    block = g_heap_base;
    while (block) {
        if (block->magic == 0xcccccccc && block->size >= size) {
            /* Found free block - use if best fit */
            if (block->size < best_size) {
                best = block;
                best_size = block->size;
            }
        }
        block = block->next;
    }
    
    if (!best) {
        /* No suitable block found */
        return NULL;
    }
    
    /* Mark block as allocated */
    best->magic = magic;
    
    return best->data;
}

/* heap_free @ 0x020349b8
 * Frees a previously allocated heap block.
 * Args: r0=pointer to allocated memory
 * Validates block is within heap bounds before freeing. */
void heap_free(void *ptr) {
    HeapBlock *block;
    
    if (!ptr) return;
    
    /* Get block header (data starts at +0x0c) */
    block = (HeapBlock*)((u8*)ptr - 0x0c);
    
    /* Validate block is within heap bounds */
    if (block < g_heap_base || block >= g_heap_end) {
        return;  /* Invalid pointer */
    }
    
    /* Validate magic number */
    if (block->magic == 0xcccccccc) {
        return;  /* Already freed */
    }
    
    /* Mark as freed */
    block->magic = 0xcccccccc;
}
