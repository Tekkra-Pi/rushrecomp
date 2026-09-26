; Touch subsystem — ARMIPS assembly sources for byte-matching.
;
; Each function is assembled at its ROM address and compared against
; the original extraction in disassembly/<addr>.bin.
;
; Usage:
;   armips touch.asm
;   # Then compare build/*.bin against disassembly/*.bin

.create "build/touch_all.bin", 0x02000000

; ──────────────────────────────────────────────────────────────
; FUN_02033d58 — TouchState_ResetEnable (80 bytes)
; Walks controller linked list, returns first active node or 0.
; ──────────────────────────────────────────────────────────────
.org 0x02033d58

ResetEnable:
    ldr     r1, [pc, #0x48]     ; DAT_02033da8 (via literal pool at end)
    ldr     r0, [pc, #0x48]     ; DAT_02033dac
    ldr     r1, [r1]            ; r1 = *DAT_02033da8
    ldr     r3, [r0]            ; r3 = *DAT_02033dac
    ldr     r0, [r1, #4]        ; r0 = first->next
    cmp     r0, r3
    beq     ResetEnable_Zero

ResetEnable_Loop:
    mvn     r1, #0              ; r1 = -1
    ldr     r2, [r0, #8]        ; r2 = node->index
    cmp     r2, r1
    ble     ResetEnable_Next    ; if index <= -1, skip
    cmp     r2, #5
    bxlt    lr                  ; if 0 <= index < 5, return node
    cmp     r2, #6
    bxgt    lr                  ; if index > 6, return node

ResetEnable_Next:
    ldr     r0, [r0, #4]        ; r0 = node->next
    cmp     r0, r3
    bne     ResetEnable_Loop

ResetEnable_Zero:
    mov     r0, #0
    bx      lr

; Literal pool (at offset 0x50 from function start)
    .word   0x02033da8          ; DAT_02033da8
    .word   0x02033dac          ; DAT_02033dac

; ──────────────────────────────────────────────────────────────
; FUN_02033ddc — TouchState_Init (96 bytes)
; Sets up touch pipeline, returns size or -1.
; Stack: 0x48 bytes. Uses r4 as temp.
; ──────────────────────────────────────────────────────────────
.org 0x02033ddc

Init:
    push    {r4, lr}
    sub     sp, sp, #0x48       ; 72 bytes stack
    mov     r4, r0              ; r4 = param
    add     r0, sp, #0          ; r0 = buf
    bl      FUN_0200be70        ; FUN_0200be70(buf)
    add     r0, sp, #0          ; r0 = buf
    mov     r1, r4              ; r1 = param
    bl      FUN_0200bb20        ; r0 = FUN_0200bb20(buf, param)
    cmp     r0, #0
    addeq   sp, sp, #0x48
    mvneq   r0, #0              ; return -1
    popeq   {r4, lr}
    bxeq    lr
    ; result != 0: compute size
    ldr     r2, [sp, #0x24]     ; r2 = buf[0x24] (p_end)
    ldr     r1, [sp, #0x20]     ; r1 = buf[0x20] (p_start)
    add     r0, sp, #0          ; r0 = buf
    sub     r4, r2, r1          ; r4 = *p_end - *p_start
    bl      FUN_0200bad8        ; r0 = FUN_0200bad8(buf)
    cmp     r0, #0
    mvneq   r4, #0              ; if check==0, size = -1
    mov     r0, r4              ; return size
    add     sp, sp, #0x48
    pop     {r4, lr}
    bx      lr

; ──────────────────────────────────────────────────────────────
; FUN_02033a68 — TouchState_MainUpdater (188 bytes)
; Optimized memory copy with alignment handling.
; r0=dst, r1=src, r2=len. Uses r4-r7.
; ──────────────────────────────────────────────────────────────
.org 0x02033a68

MainUpdater:
    push    {r4, r5, r6, r7, lr}
    sub     sp, sp, #4
    mov     r6, r0              ; r6 = dst
    mov     r5, r1              ; r5 = src
    orr     r3, r6, r5          ; r3 = dst|src
    mov     r4, r2              ; r4 = len
    ands    r2, r3, #3          ; r2 = (dst|src)&3
    bne     MainUpdater_Check1  ; if not 4-byte aligned, skip 32/4-byte chunks

    ; 32-byte aligned chunks
    bics    r7, r4, #31         ; r7 = len & ~31
    beq     MainUpdater_4byte   ; if 0, skip
    mov     r2, r7
    bl      FUN_020083ac        ; FUN_020083ac(dst, src, n32)
    add     r6, r6, r7          ; dst += n32
    add     r5, r5, r7          ; src += n32
    and     r4, r4, #31         ; len &= 31

MainUpdater_4byte:
    ; 4-byte aligned chunks
    bics    r7, r4, #3          ; r7 = len & ~3
    beq     MainUpdater_Check1  ; if 0, skip
    mov     r0, r6
    mov     r1, r5
    mov     r2, r7
    bl      FUN_02008330        ; FUN_02008330(dst, src, n4)
    add     r6, r6, r7
    add     r5, r5, r7
    and     r4, r4, #3          ; len &= 3

MainUpdater_Check1:
    ; 2-byte aligned check
    orr     r0, r6, r5
    ands    r0, r0, #1
    bne     MainUpdater_1byte   ; if either is odd, skip 2-byte
    bics    r7, r4, #1          ; r7 = len & ~1
    beq     MainUpdater_1byte
    mov     r0, r6
    mov     r1, r5
    mov     r2, r7
    bl      FUN_02008300        ; FUN_02008300(dst, src, n2)
    add     r6, r6, r7
    add     r5, r5, r7
    and     r4, r4, #1          ; len &= 1

MainUpdater_1byte:
    cmp     r4, #0
    addeq   sp, sp, #4
    popeq   {r4, r5, r6, r7, lr}
    bxeq    lr
    ; 1-byte remainder
    mov     r0, r6
    mov     r1, r5
    mov     r2, r4
    bl      FUN_02008500        ; FUN_02008500(dst, src, len)
    add     sp, sp, #4
    pop     {r4, r5, r6, r7, lr}
    bx      lr

; ──────────────────────────────────────────────────────────────
; External function stubs (relative bl targets, resolved at link)
; These are placeholders — the actual bl offsets are ROM-specific.
; ──────────────────────────────────────────────────────────────
FUN_0200be70:
FUN_0200bb20:
FUN_0200bad8:
FUN_020083ac:
FUN_02008330:
FUN_02008300:
FUN_02008500:
    bx      lr  ; stub

.close
