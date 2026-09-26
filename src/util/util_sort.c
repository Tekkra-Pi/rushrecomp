/* Utility sort functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 *
 * Swap function used by various sorting algorithms (bubble, insertion, quick).
 */

#include "nds_types.h"

/* ======================================================================== */
/* Sort_Swap                                                                 */
/* Swaps two elements in an array given their element size.                 */
/* Args: r0=data base, r1=index A, r2=index B, r3=element_size              */
/* Uses byte-by-byte swap for arbitrary element sizes.                      */
/* ======================================================================== */
void Sort_Swap(void *data, u32 a, u32 b, u32 element_size) {
    u8 *base = (u8 *)data;
    u8 *ptr_a = base + a * element_size;
    u8 *ptr_b = base + b * element_size;
    u32 i;

    for (i = 0; i < element_size; i++) {
        u8 tmp = ptr_a[i];
        ptr_a[i] = ptr_b[i];
        ptr_b[i] = tmp;
    }
}
