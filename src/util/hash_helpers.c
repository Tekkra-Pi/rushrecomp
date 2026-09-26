

#include "nds_types.h"

/* Hash/resource system helpers used by touch_init.
 * Evidence: INFERRED from touch_init.c usage and disassembly */

/* Hash structure (0x1c bytes) */
typedef struct {
    u32 field_00;           /* +0x00: initialized to 0 */
    u32 field_04;           /* +0x04: initialized to 0 */
    u32 field_08;           /* +0x08: initialized to 0 */
    u32 field_0c;           /* +0x0c: flags, bits 4-5 cleared by hash_parse */
    u32 field_10;           /* +0x10: initialized to 0xe */
    u32 field_14;           /* +0x14: unknown */
    u16 field_18;           /* +0x18: initialized to 0 */
} HashStruct;

/* hash_init @ 0x0200be70
 * Initializes a hash structure.
 * Args: r0=buf (HashStruct pointer)
 * Sets field_10 = 0xe, all others to 0. */
void hash_init(HashStruct *buf) {
    buf->field_00 = 0;
    buf->field_04 = 0;
    buf->field_08 = 0;
    buf->field_0c = 0;
    buf->field_10 = 0xe;
    buf->field_18 = 0;
}

/* hash_parse @ 0x0200bad8
 * Allocates and initializes a hash-related structure.
 * Args: r0=buf (HashStruct pointer)
 * Returns: 1 on success, 0 on allocation failure. */
int hash_parse(HashStruct *buf) {
    void *alloc;
    
    alloc = malloc(8);
    if (!alloc) {
        return 0;
    }
    
    buf->field_08 = 0;
    buf->field_10 = 0xe;
    buf->field_0c &= ~0x30;  /* Clear bits 4-5 */
    
    return 1;
}

/* hash_search @ 0x0200bb20
 * Searches for a hash/resource.
 * Args: r0=buf (HashStruct pointer)
 * Returns: 1 if found, 0 if not found. */
int hash_search(HashStruct *buf) {
    u32 result[2];
    
    if (!hash_lookup(result)) {
        return 0;
    }
    
    if (!hash_verify(buf, result[0], result[1])) {
        return 0;
    }
    
    return 1;
}
