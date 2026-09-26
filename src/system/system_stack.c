/* System stack functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* Stack_Init @ 0x0200c800 (168 bytes)
 * Initializes stack system. */
void Stack_Init(void) {
    /* Clear stack state */
    g_stack_pointer = 0;
    g_stack_base = 0;
    g_stack_size = 0;
}

/* Stack_Push @ 0x0200c8a8 (232 bytes)
 * Pushes value onto stack.
 * Args: r0=value
 * Returns: 1 if success, 0 if overflow */
s32 Stack_Push(u32 value) {
    if (g_stack_pointer >= g_stack_size) {
        return 0;  /* Stack overflow */
    }
    
    g_stack_base[g_stack_pointer] = value;
    g_stack_pointer++;
    
    return 1;
}

/* Stack_Pop @ 0x0200c990 (296 bytes)
 * Pops value from stack.
 * Returns: popped value or 0 if empty */
u32 Stack_Pop(void) {
    if (g_stack_pointer == 0) {
        return 0;  /* Stack underflow */
    }
    
    g_stack_pointer--;
    return g_stack_base[g_stack_pointer];
}

/* Stack_Peek @ 0x0200cab8
 * Returns top of stack without popping.
 * Returns: top value or 0 if empty */
u32 Stack_Peek(void) {
    if (g_stack_pointer == 0) {
        return 0;
    }
    
    return g_stack_base[g_stack_pointer - 1];
}

/* Stack_IsEmpty @ 0x0200cb00
 * Returns whether stack is empty.
 * Returns: 1 if empty, 0 otherwise */
s32 Stack_IsEmpty(void) {
    return g_stack_pointer == 0;
}

/* Stack_IsFull @ 0x0200cb20
 * Returns whether stack is full.
 * Returns: 1 if full, 0 otherwise */
s32 Stack_IsFull(void) {
    return g_stack_pointer >= g_stack_size;
}

/* Stack_GetSize @ 0x0200cb40
 * Returns number of items in stack.
 * Returns: stack size */
u32 Stack_GetSize(void) {
    return g_stack_pointer;
}

/* Stack_Clear @ 0x0200cb60
 * Clears the stack. */
void Stack_Clear(void) {
    g_stack_pointer = 0;
}
