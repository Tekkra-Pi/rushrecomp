/* Slide and grind functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 * Decompiled from ARM9 binary in player subsystem range. */

#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

extern void Sound_PlayEffect(u32 id);
extern void DustEffect_Spawn(s32 x, s32 y, u32 type);

/* Forward declarations */
void Slide_End(void);
void Grind_End(void);

/* ======================================================================== */
/* Slide_Init @ 0x02064554+                                                 */
/* Initializes slide state: sets timer, direction, marks busy.              */
/* ======================================================================== */
void Slide_Init(void)
{
    PhysicsPlayer* p = g_current_player;
    p->slide_timer = 120;
    p->slide_direction = p->direction;
    p->control_flags |= PLAYER_CTRL_BUSY;
    p->state_flags |= PLAYER_STATE_S;
}

/* ======================================================================== */
/* Slide_Update @ 0x02064554+ (within large bin)                            */
/* Per-frame slide update: decrements timer, spawns dust, ends at 0.        */
/* ======================================================================== */
void Slide_Update(void)
{
    PhysicsPlayer* p = g_current_player;

    if (p->slide_timer > 0)
    {
        p->slide_timer--;

        /* Spawn slide dust effect periodically */
        if ((p->slide_timer & 0xF) == 0)
        {
            DustEffect_Spawn(p->pos_x, p->pos_y, 0);
        }

        if (p->slide_timer == 0)
        {
            Slide_End();
        }
    }
}

/* ======================================================================== */
/* Slide_End                                                                */
/* Cleans up slide state.                                                   */
/* ======================================================================== */
void Slide_End(void)
{
    PhysicsPlayer* p = g_current_player;
    p->slide_timer = 0;
    p->slide_direction = 0;
    p->control_flags &= ~PLAYER_CTRL_BUSY;
    p->state_flags &= ~PLAYER_STATE_S;
}

/* ======================================================================== */
/* Grind_Init @ 0x020646bc+                                                 */
/* Initializes grind state: sets direction, marks busy.                     */
/* ======================================================================== */
void Grind_Init(void)
{
    PhysicsPlayer* p = g_current_player;
    p->grind_timer = 0;
    p->grind_direction = p->direction;
    p->control_flags |= PLAYER_CTRL_BUSY;
    p->state_flags |= PLAYER_STATE_T;
}

/* ======================================================================== */
/* Grind_Update @ 0x020646bc+                                               */
/* Per-frame grind update: increments timer, checks grind rail contact.     */
/* ======================================================================== */
void Grind_Update(void)
{
    PhysicsPlayer* p = g_current_player;

    p->grind_timer++;

    /* Apply grind velocity based on direction */
    p->applied_vel_x = (s16)(p->grind_direction * 0x200);

    /* End grind if no longer on rail */
    if (!(p->state_flags & PLAYER_STATE_GROUNDED))
    {
        Grind_End();
    }
}

/* ======================================================================== */
/* Grind_End                                                                */
/* Cleans up grind state.                                                   */
/* ======================================================================== */
void Grind_End(void)
{
    PhysicsPlayer* p = g_current_player;
    p->grind_timer = 0;
    p->grind_direction = 0;
    p->control_flags &= ~PLAYER_CTRL_BUSY;
    p->state_flags &= ~PLAYER_STATE_T;
}
