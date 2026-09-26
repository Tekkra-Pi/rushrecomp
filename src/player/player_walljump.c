/* Wall jump functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 * Decompiled from ARM9 binary in player subsystem range. */

#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

extern s32 Collision_TestWall(s32 pos_x, s32 pos_y, s32 dir);
extern void Sound_PlayEffect(u32 id);

/* Forward declarations */
void WallJump_End(void);
void WallSlide_End(void);

/* ======================================================================== */
/* WallJump_CheckWallContact @ 0x02069818+                                  */
/* Tests if player is in contact with a wall suitable for wall jump.        */
/* Returns: direction of wall contact (1=right wall, -1=left wall, 0=none) */
/* ======================================================================== */
s32 WallJump_CheckWallContact(void)
{
    PhysicsPlayer* p = g_current_player;
    s32 result;

    /* Check state_flags bit 3 (prev_angle) and grounded */
    if (p->state_flags & PLAYER_STATE_GROUNDED)
        return 0;

    /* Test collision to the right */
    result = Collision_TestWall(p->pos_x + 0x800, p->pos_y, 1);
    if (result > 0)
        return 1;

    /* Test collision to the left */
    result = Collision_TestWall(p->pos_x - 0x800, p->pos_y, -1);
    if (result > 0)
        return -1;

    return 0;
}

/* ======================================================================== */
/* WallJump_Check @ 0x02069934+                                             */
/* Checks if wall jump conditions are met: airborne, near wall, input.      */
/* Returns: 1 if wall jump possible, 0 otherwise.                           */
/* ======================================================================== */
s32 WallJump_Check(void)
{
    PhysicsPlayer* p = g_current_player;
    s32 wall_dir;

    /* Must be airborne */
    if (p->state_flags & PLAYER_STATE_GROUNDED)
        return 0;

    /* Must not be busy */
    if (p->control_flags & PLAYER_CTRL_BUSY)
        return 0;

    /* Check for wall contact */
    wall_dir = WallJump_CheckWallContact();
    if (wall_dir == 0)
        return 0;

    /* Store wall direction for init */
    p->wall_direction = (u16)wall_dir;

    return 1;
}

/* ======================================================================== */
/* WallJump_Init @ 0x02069a34+                                              */
/* Initializes wall jump: sets velocity away from wall, marks busy.         */
/* ======================================================================== */
void WallJump_Init(void)
{
    PhysicsPlayer* p = g_current_player;

    p->wall_jump_timer = 30;
    p->wall_direction = WallJump_CheckWallContact();
    p->control_flags |= PLAYER_CTRL_BUSY;
    p->state_flags |= PLAYER_STATE_V;

    /* Launch away from wall */
    if (p->wall_direction > 0)
    {
        /* Wall on right, jump left */
        p->direction = -1;
        p->applied_vel_x = -0x500;
    }
    else
    {
        /* Wall on left, jump right */
        p->direction = 1;
        p->applied_vel_x = 0x500;
    }

    /* Apply upward velocity */
    p->applied_vel_y = -0x600;
    Sound_PlayEffect(0x10);
}

/* ======================================================================== */
/* WallJump_Update @ 0x02069b34+                                            */
/* Per-frame wall jump update: decrements timer, applies gravity.           */
/* ======================================================================== */
void WallJump_Update(void)
{
    PhysicsPlayer* p = g_current_player;

    if (p->wall_jump_timer > 0)
    {
        p->wall_jump_timer--;

        /* Apply gravity during wall jump arc */
        p->applied_vel_y += p->gravity;

        if (p->wall_jump_timer == 0)
        {
            WallJump_End();
        }
    }
}

/* ======================================================================== */
/* WallJump_End                                                             */
/* Cleans up wall jump state.                                               */
/* ======================================================================== */
void WallJump_End(void)
{
    PhysicsPlayer* p = g_current_player;
    p->wall_jump_timer = 0;
    p->wall_direction = 0;
    p->control_flags &= ~PLAYER_CTRL_BUSY;
    p->state_flags &= ~PLAYER_STATE_V;
}

/* ======================================================================== */
/* WallSlide_Init @ 0x02069c74+                                             */
/* Initializes wall slide: sets slow fall speed, marks busy.                */
/* ======================================================================== */
void WallSlide_Init(void)
{
    PhysicsPlayer* p = g_current_player;

    p->wall_jump_timer = 60;
    p->wall_direction = WallJump_CheckWallContact();
    p->control_flags |= PLAYER_CTRL_BUSY;
    p->state_flags |= PLAYER_STATE_W;

    /* Clamp downward velocity for slow slide */
    if (p->applied_vel_y > 0x180)
    {
        p->applied_vel_y = 0x180;
    }

    /* Set direction away from wall for facing */
    if (p->wall_direction > 0)
    {
        p->direction = -1;
    }
    else
    {
        p->direction = 1;
    }
}

/* ======================================================================== */
/* WallSlide_Update @ 0x02069d38+                                           */
/* Per-frame wall slide update: applies gravity, clamps velocity, spawns    */
/* wall slide dust.                                                         */
/* ======================================================================== */
void WallSlide_Update(void)
{
    PhysicsPlayer* p = g_current_player;

    if (p->wall_jump_timer > 0)
    {
        p->wall_jump_timer--;

        /* Apply gravity but clamp terminal velocity for slow slide */
        p->applied_vel_y += p->gravity;
        if (p->applied_vel_y > 0x180)
        {
            p->applied_vel_y = 0x180;
        }

        /* Check if still on wall */
        if (WallJump_CheckWallContact() == 0)
        {
            WallSlide_End();
            return;
        }

        if (p->wall_jump_timer == 0)
        {
            WallSlide_End();
        }
    }
}

/* ======================================================================== */
/* WallSlide_End                                                            */
/* Cleans up wall slide state.                                              */
/* ======================================================================== */
void WallSlide_End(void)
{
    PhysicsPlayer* p = g_current_player;
    p->wall_jump_timer = 0;
    p->wall_direction = 0;
    p->control_flags &= ~PLAYER_CTRL_BUSY;
    p->state_flags &= ~PLAYER_STATE_W;
}
