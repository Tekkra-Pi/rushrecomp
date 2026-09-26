/* Utility search functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* Search_Linear @ 0x0200e800 (168 bytes)
 * Linear search implementation.
 * Args: r0=data, r1=count, r2=key, r3=comparator
 * Returns: index or -1 */
s32 Search_Linear(void *data, u32 count, void *key, s32 (*comparator)(void*, void*)) {
    u32 *arr = (u32*)data;
    
    for (u32 i = 0; i < count; i++) {
        if (comparator(&arr[i], key) == 0) {
            return i;
        }
    }
    
    return -1;
}

/* Search_Binary @ 0x0200e8a8 (232 bytes)
 * Binary search implementation.
 * Args: r0=data, r1=count, r2=key, r3=comparator
 * Returns: index or -1 */
s32 Search_Binary(void *data, u32 count, void *key, s32 (*comparator)(void*, void*)) {
    u32 *arr = (u32*)data;
    s32 low = 0;
    s32 high = count - 1;
    
    while (low <= high) {
        s32 mid = (low + high) / 2;
        s32 result = comparator(&arr[mid], key);
        
        if (result == 0) {
            return mid;
        } else if (result < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    return -1;
}

/* Search_FindFirst @ 0x0200e990 (296 bytes)
 * Finds first matching element.
 * Args: r0=data, r1=count, r2=predicate
 * Returns: pointer to first match or NULL */
void* Search_FindFirst(void *data, u32 count, s32 (*predicate)(void*)) {
    u32 *arr = (u32*)data;
    
    for (u32 i = 0; i < count; i++) {
        if (predicate(&arr[i])) {
            return &arr[i];
        }
    }
    
    return NULL;
}

/* Search_FindAll @ 0x0200eab8
 * Finds all matching elements.
 * Args: r0=data, r1=count, r2=predicate, r3=output, sp=max_results
 * Returns: number of matches */
u32 Search_FindAll(void *data, u32 count, s32 (*predicate)(void*), void *output, u32 max_results) {
    u32 *arr = (u32*)data;
    u32 *out = (u32*)output;
    u32 found = 0;
    
    for (u32 i = 0; i < count && found < max_results; i++) {
        if (predicate(&arr[i])) {
            out[found] = arr[i];
            found++;
        }
    }
    
    return found;
}

/* Search_Count @ 0x0200eb60
 * Counts matching elements.
 * Args: r0=data, r1=count, r2=predicate
 * Returns: count of matches */
u32 Search_Count(void *data, u32 count, s32 (*predicate)(void*)) {
    u32 *arr = (u32*)data;
    u32 matches = 0;
    
    for (u32 i = 0; i < count; i++) {
        if (predicate(&arr[i])) {
            matches++;
        }
    }
    
    return matches;
}

/* Search_Min @ 0x0200ebc0
 * Finds minimum element.
 * Args: r0=data, r1=count, r2=element_size, r3=comparator
 * Returns: pointer to minimum */
void* Search_Min(void *data, u32 count, u32 element_size, s32 (*comparator)(void*, void*)) {
    u8 *base = (u8*)data;
    void *min = base;
    
    for (u32 i = 1; i < count; i++) {
        void *current = base + (i * element_size);
        if (comparator(current, min) < 0) {
            min = current;
        }
    }
    
    return min;
}

/* Search_Max @ 0x0200ec60
 * Finds maximum element.
 * Args: r0=data, r1=count, r2=element_size, r3=comparator
 * Returns: pointer to maximum */
void* Search_Max(void *data, u32 count, u32 element_size, s32 (*comparator)(void*, void*)) {
    u8 *base = (u8*)data;
    void *max = base;
    
    for (u32 i = 1; i < count; i++) {
        void *current = base + (i * element_size);
        if (comparator(current, max) > 0) {
            max = current;
        }
    }
    
    return max;
}

/* Search_BinaryIndex @ 0x0200ed00
 * Binary search for sorted index.
 * Args: r0=data, r1=count, r2=key, r3=comparator
 * Returns: insertion point */
u32 Search_BinaryIndex(void *data, u32 count, void *key, s32 (*comparator)(void*, void*)) {
    u32 *arr = (u32*)data;
    s32 low = 0;
    s32 high = count - 1;
    
    while (low <= high) {
        s32 mid = (low + high) / 2;
        s32 result = comparator(&arr[mid], key);
        
        if (result < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    return low;
}
