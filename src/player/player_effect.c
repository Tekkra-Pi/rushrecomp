/* Player effect functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 * Decompiled from ARM9 binary in player subsystem range. */

#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

#define MAX_EFFECTS 16

/* Effect entry structure */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 timer;
    u32 active;
    u32 param;
} PlayerEffect;

/* Effect state */
static PlayerEffect s_effects[MAX_EFFECTS];

extern void OAM_SetSprite(u32 id, s32 x, s32 y, u32 tile, u32 attr);
extern u32 g_oam_count;

/* ======================================================================== */
/* PlayerEffect_Init                                                        */
/* Initializes the player effect system.                                    */
/* ======================================================================== */
void PlayerEffect_Init(void)
{
    u32 i;
    for (i = 0; i < MAX_EFFECTS; i++)
    {
        s_effects[i].active = 0;
        s_effects[i].timer = 0;
        s_effects[i].type = 0;
    }
}

/* ======================================================================== */
/* PlayerEffect_Add                                                         */
/* Adds a new player effect at given position.                              */
/* Args: x, y = position (fixed-point 8.8), param = effect type            */
/* ======================================================================== */
void PlayerEffect_Add(s32 x, s32 y, u32 param)
{
    u32 i;

    for (i = 0; i < MAX_EFFECTS; i++)
    {
        if (!s_effects[i].active)
        {
            s_effects[i].x = x;
            s_effects[i].y = y;
            s_effects[i].type = param;
            s_effects[i].timer = 0;
            s_effects[i].active = 1;
            s_effects[i].param = 0;
            return;
        }
    }
}

/* ======================================================================== */
/* PlayerEffect_Remove                                                      */
/* Removes a player effect by index.                                        */
/* ======================================================================== */
void PlayerEffect_Remove(u32 index)
{
    if (index < MAX_EFFECTS)
    {
        s_effects[index].active = 0;
        s_effects[index].timer = 0;
    }
}

/* ======================================================================== */
/* PlayerEffect_Update                                                      */
/* Per-frame update for all active player effects.                          */
/* ======================================================================== */
void PlayerEffect_Update(void)
{
    u32 i;

    for (i = 0; i < MAX_EFFECTS; i++)
    {
        if (s_effects[i].active)
        {
            s_effects[i].timer++;

            /* Type-specific update */
            switch (s_effects[i].type)
            {
            case 0: /* sparkle */
                if (s_effects[i].timer > 20)
                    s_effects[i].active = 0;
                break;
            case 1: /* trail */
                if (s_effects[i].timer > 30)
                    s_effects[i].active = 0;
                break;
            default:
                if (s_effects[i].timer > 40)
                    s_effects[i].active = 0;
                break;
            }
        }
    }
}

/* ======================================================================== */
/* PlayerEffect_UpdateType                                                  */
/* Updates effects of a specific type.                                      */
/* ======================================================================== */
void PlayerEffect_UpdateType(u32 type)
{
    u32 i;

    for (i = 0; i < MAX_EFFECTS; i++)
    {
        if (s_effects[i].active && s_effects[i].type == type)
        {
            s_effects[i].timer++;
        }
    }
}

/* ======================================================================== */
/* PlayerEffect_UpdateInvincibility                                         */
/* Updates invincibility sparkle effects.                                   */
/* ======================================================================== */
void PlayerEffect_UpdateInvincibility(void)
{
    PhysicsPlayer* p = g_current_player;

    if (p->invuln_timer > 0)
    {
        p->invuln_timer--;

        /* Spawn sparkle at player position periodically */
        if ((p->invuln_timer & 0x3) == 0)
        {
            PlayerEffect_Add(p->pos_x, p->pos_y, 0);
        }
    }
}

/* ======================================================================== */
/* PlayerEffect_UpdateSpeedBoost                                            */
/* Updates speed boost visual effects.                                      */
/* ======================================================================== */
void PlayerEffect_UpdateSpeedBoost(void)
{
    PhysicsPlayer* p = g_current_player;

    /* Check state_flags for speed boost */
    if (p->state_flags & PLAYER_STATE_DIR_100000)
    {
        /* Spawn speed lines behind player */
        if ((p->action_timer & 0x3) == 0)
        {
            s32 trail_x = p->pos_x - (p->direction * 0x400);
            PlayerEffect_Add(trail_x, p->pos_y, 1);
        }
    }
}

/* ======================================================================== */
/* PlayerEffect_UpdateShield                                                */
/* Updates shield visual effects.                                           */
/* ======================================================================== */
void PlayerEffect_UpdateShield(void)
{
    PhysicsPlayer* p = g_current_player;

    if (p->state_flags & PLAYER_STATE_FROZEN)
    {
        /* Shield active: update shield sparkle */
        if ((p->action_timer & 0x7) == 0)
        {
            PlayerEffect_Add(p->pos_x, p->pos_y - 0x200, 2);
        }
    }
}

/* ======================================================================== */
/* PlayerEffect_Has                                                         */
/* Checks if a specific effect type is active.                              */
/* Returns: 1 if effect of given type exists, 0 otherwise.                  */
/* ======================================================================== */
s32 PlayerEffect_Has(void)
{
    u32 i;
    u32 type = g_current_player->state_flags & 0xFF;

    for (i = 0; i < MAX_EFFECTS; i++)
    {
        if (s_effects[i].active && s_effects[i].type == type)
        {
            return 1;
        }
    }

    return 0;
}

/* ======================================================================== */
/* PlayerEffect_Clear                                                       */
/* Clears all active player effects.                                        */
/* ======================================================================== */
void PlayerEffect_Clear(void)
{
    u32 i;
    for (i = 0; i < MAX_EFFECTS; i++)
    {
        s_effects[i].active = 0;
        s_effects[i].timer = 0;
    }
}
