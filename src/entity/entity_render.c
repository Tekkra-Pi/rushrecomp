/* Entity render functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 *
 * EntityRender manages OAM sprite attributes for entity rendering.
 * Uses a global OAM shadow buffer that is copied to hardware OAM
 * during the VBlank transfer.
 */

#include "nds_types.h"

/* OAM shadow buffer base (at 0x0208922c or similar) */
extern u16 g_oam_shadow[];

/* Global render state */
extern u32 g_render_flags;
extern u32 g_render_index;

/* ======================================================================== */
/* EntityRender_Init                                                         */
/* Initializes the entity render system by clearing the OAM shadow buffer   */
/* and resetting render state.                                              */
/* ======================================================================== */
void EntityRender_Init(void) {
    u32 i;

    /* Clear OAM shadow (128 sprites * 4 halfwords = 256 halfwords) */
    for (i = 0; i < 256; i++) {
        g_oam_shadow[i] = 0;
    }

    g_render_flags = 0;
    g_render_index = 0;
}

/* ======================================================================== */
/* EntityRender_SetSprite                                                    */
/* Sets the sprite tile/protection attributes for an OAM entry.            */
/* Args: r0=index (OAM entry 0-127), r1=value (attr2: tile + priority)     */
/* ======================================================================== */
void EntityRender_SetSprite(u32 index, u32 value) {
    if (index < 128) {
        g_oam_shadow[index * 4 + 2] = (u16)value;
    }
}

/* ======================================================================== */
/* EntityRender_Update                                                       */
/* Per-frame update: copies the OAM shadow buffer to hardware OAM.         */
/* Called during VBlank processing.                                         */
/* ======================================================================== */
void EntityRender_Update(void) {
    u32 i;
    volatile u16 *oam = OAM;

    for (i = 0; i < 256; i++) {
        oam[i] = g_oam_shadow[i];
    }
}

/* ======================================================================== */
/* EntityRender_Draw                                                         */
/* Final draw step: applies any pending render state changes.              */
/* Typically called after all entity sprites have been configured.          */
/* ======================================================================== */
void EntityRender_Draw(void) {
    /* Mark render as complete; pending copies happen in Update */
    g_render_flags |= 1;
    return;
}

/* ======================================================================== */
/* EntityRender_SetFrame                                                     */
/* Sets the frame/tile index for an OAM entry.                              */
/* Args: r0=index, r1=frame (tile number)                                   */
/* ======================================================================== */
void EntityRender_SetFrame(u32 index, u32 value) {
    if (index < 128) {
        u16 attr2 = g_oam_shadow[index * 4 + 2];
        attr2 = (attr2 & 0xFC00) | (u16)(value & 0x03FF);
        g_oam_shadow[index * 4 + 2] = attr2;
    }
}

/* ======================================================================== */
/* EntityRender_SetFlags                                                     */
/* Sets the shape/size attributes for an OAM entry (attr0/attr1 flags).    */
/* Args: r0=index, r1=flags (attr0 lower bits)                             */
/* ======================================================================== */
void EntityRender_SetFlags(u32 index, u32 value) {
    if (index < 128) {
        u16 attr0 = g_oam_shadow[index * 4 + 0];
        attr0 = (attr0 & 0xFF00) | (u16)(value & 0x00FF);
        g_oam_shadow[index * 4 + 0] = attr0;
    }
}

/* ======================================================================== */
/* EntityRender_SetFlip                                                      */
/* Sets horizontal/vertical flip for an OAM entry.                          */
/* Args: r0=index, r1=flip (bit0=hflip, bit1=vflip)                        */
/* ======================================================================== */
void EntityRender_SetFlip(u32 index, u32 value) {
    if (index < 128) {
        u16 attr1 = g_oam_shadow[index * 4 + 1];
        attr1 = (attr1 & 0x0FFF) | (u16)((value & 0x03) << 12);
        g_oam_shadow[index * 4 + 1] = attr1;
    }
}

/* ======================================================================== */
/* EntityRender_SetAlpha                                                     */
/* Sets the alpha/blending mode for an OAM entry.                           */
/* Args: r0=index, r1=alpha (bit10 of attr0 = semi-transparent)            */
/* ======================================================================== */
void EntityRender_SetAlpha(u32 index, u32 value) {
    if (index < 128) {
        u16 attr0 = g_oam_shadow[index * 4 + 0];
        if (value) {
            attr0 |= (1 << 10);
        } else {
            attr0 &= ~(1 << 10);
        }
        g_oam_shadow[index * 4 + 0] = attr0;
    }
}

/* ======================================================================== */
/* EntityRender_SetPriority                                                  */
/* Sets the OAM priority for an OAM entry (attr2 bits 10-11).              */
/* Args: r0=index, r1=priority (0-3)                                        */
/* ======================================================================== */
void EntityRender_SetPriority(u32 index, u32 value) {
    if (index < 128) {
        u16 attr2 = g_oam_shadow[index * 4 + 2];
        attr2 = (attr2 & 0xCFFF) | (u16)((value & 0x03) << 10);
        g_oam_shadow[index * 4 + 2] = attr2;
    }
}

/* ======================================================================== */
/* EntityRender_Hide                                                         */
/* Hides an OAM entry by setting the disable bit (bit8 of attr0).          */
/* ======================================================================== */
void EntityRender_Hide(void) {
    u32 idx = g_render_index;
    if (idx < 128) {
        g_oam_shadow[idx * 4 + 0] |= (1 << 8);
    }
}

/* ======================================================================== */
/* EntityRender_Show                                                         */
/* Shows an OAM entry by clearing the disable bit (bit8 of attr0).         */
/* ======================================================================== */
void EntityRender_Show(void) {
    u32 idx = g_render_index;
    if (idx < 128) {
        g_oam_shadow[idx * 4 + 0] &= ~(1 << 8);
    }
}
