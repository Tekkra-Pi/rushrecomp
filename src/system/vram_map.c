/* VRAM mapping functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* VRAM mapping table at 0x2089240 */
extern u32 g_vram_mapping[];

/* VRAM_Map @ 0x0203a000 (188 bytes)
 * Maps VRAM bank to specified address.
 * Args: r0=bank_id, r1=address */
void VRAM_Map(u32 bank_id, u32 address) {
    u8 *cr = (u8*)&VRAM_A_CR + bank_id;
    
    /* Set bank MSTAT */
    switch (bank_id) {
        case 0:  /* VRAM A */
            VRAM_A_CR = (address >> 24) & 0x03;
            break;
        case 1:  /* VRAM B */
            VRAM_B_CR = (address >> 24) & 0x03;
            break;
        case 2:  /* VRAM C */
            VRAM_C_CR = (address >> 24) & 0x03;
            break;
        case 3:  /* VRAM D */
            VRAM_D_CR = (address >> 24) & 0x03;
            break;
        default:
            break;
    }
}

/* VRAM_Unmap @ 0x0203a0bc (256 bytes)
 * Unmaps VRAM bank.
 * Args: r0=bank_id */
void VRAM_Unmap(u32 bank_id) {
    u8 *cr = (u8*)&VRAM_A_CR + bank_id;
    
    /* Disable bank */
    *cr &= ~0x80;
}

/* VRAM_GetMapping @ 0x0203a1bc (296 bytes)
 * Gets current VRAM bank mapping.
 * Args: r0=bank_id
 * Returns: mapped address or -1 */
u32 VRAM_GetMapping(u32 bank_id) {
    u8 *cr = (u8*)&VRAM_A_CR + bank_id;
    
    if (*cr & 0x80) {
        /* Bank is enabled */
        u32 mapping = g_vram_mapping[bank_id];
        return mapping;
    }
    
    return 0xffffffff;  /* Not mapped */
}

/* VRAM_Init @ 0x0203a300
 * Initializes VRAM banks with default mapping. */
void VRAM_Init(void) {
    /* Map VRAM A to BG */
    VRAM_Map(0, 0x06000000);
    
    /* Map VRAM B to OBJ */
    VRAM_Map(1, 0x06400000);
    
    /* Map VRAM C to BG */
    VRAM_Map(2, 0x06020000);
    
    /* Map VRAM D to BG */
    VRAM_Map(3, 0x06040000);
}
