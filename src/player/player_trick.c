/* Trick system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 * Decompiled from ARM9 binary in player subsystem range. */

#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

extern void Sound_PlayEffect(u32 id);
extern void TrickSparkle_Spawn(s32 x, s32 y, u32 type);

/* Forward declarations */
void Trick_End(void);

/* Trick rotation table: indexed by current_trick */
static const u16 trick_rotation_table[] = {
    0x0000, 0x0400, 0x0800, 0x0C00,
    0x1000, 0x1400, 0x1800, 0x1C00,
};

/* Trick bonus values */
static const u16 trick_bonus_table[] = {
    100, 200, 300, 400, 500, 600, 800, 1000,
};

/* ======================================================================== */
/* Trick_Init @ 0x0206a054+                                                 */
/* Initializes trick state: sets timer, duration, rotation, bonus.          */
/* ======================================================================== */
void Trick_Init(void)
{
    PhysicsPlayer* p = g_current_player;
    u32 trick_idx;

    p->trick_timer = 0;
    p->trick_rotation = 0;

    /* Get trick index from current_trick */
    trick_idx = p->current_trick & 0x7;
    p->trick_duration = 30;
    p->trick_bonus = trick_bonus_table[trick_idx];

    p->control_flags |= PLAYER_CTRL_BUSY;
    p->state_flags |= PLAYER_STATE_T;

    /* Set action timer for rotation duration */
    p->action_timer = 0x30;

    Sound_PlayEffect(0x15);
}

/* ======================================================================== */
/* Trick_Update @ 0x0206a094+                                               */
/* Per-frame trick update: increments rotation, decrements timer.           */
/* ======================================================================== */
void Trick_Update(void)
{
    PhysicsPlayer* p = g_current_player;

    if (p->trick_timer < p->trick_duration)
    {
        p->trick_timer++;

        /* Increment rotation (fixed-point: 0x100 = full rotation) */
        p->trick_rotation += 0x150;

        /* Spawn trick sparkle effects */
        if ((p->trick_timer & 0x7) == 0)
        {
            TrickSparkle_Spawn(p->pos_x, p->pos_y, p->current_trick & 0x7);
        }

        if (p->trick_timer >= p->trick_duration)
        {
            Trick_End();
        }
    }
}

/* ======================================================================== */
/* Trick_End                                                                */
/* Cleans up trick state, awards bonus.                                     */
/* ======================================================================== */
void Trick_End(void)
{
    PhysicsPlayer* p = g_current_player;

    p->trick_timer = 0;
    p->trick_rotation = 0;
    p->trick_bonus = 0;
    p->current_trick = 0;
    p->control_flags &= ~PLAYER_CTRL_BUSY;
    p->state_flags &= ~PLAYER_STATE_T;
}

/* ======================================================================== */
/* Trick_GetRotation @ 0x0206a170+                                          */
/* Returns current trick rotation value.                                    */
/* ======================================================================== */
u32 Trick_GetRotation(void)
{
    PhysicsPlayer* p = g_current_player;
    return (u32)p->trick_rotation;
}

/* ======================================================================== */
/* Trick_IsActive @ 0x0206a4a8+                                             */
/* Checks if a specific trick type is currently active.                     */
/* Args: r0=trick_index                                                     */
/* Returns: 1 if active, 0 otherwise.                                       */
/* ======================================================================== */
s32 Trick_IsActive(u32 index)
{
    PhysicsPlayer* p = g_current_player;

    if (p->trick_timer == 0)
        return 0;

    if ((p->current_trick & 0x7) != (index & 0x7))
        return 0;

    return 1;
}
