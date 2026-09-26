/* Pause controller and player spawn functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 671-710
 * Decompiled from ARM9 binary in player subsystem range. */

#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

extern void Sprite_SetVisible(u32 id, u32 visible);
extern void OAM_SetSprite(u32 id, s32 x, s32 y, u32 tile, u32 attr);
extern void Player_Spawn(s32 pos_x, s32 pos_y);
extern u32 g_oam_count;
extern u16 g_oam_buffer[];
extern u32 g_game_paused;
extern u32 g_system_flags;

/* Pause controller state */
static u32 s_pause_active = 0;
static u32 s_pause_sprite_id = 0;

/* ======================================================================== */
/* PauseController_Init                                                     */
/* Initializes the pause controller overlay.                                */
/* ======================================================================== */
void PauseController_Init(void)
{
    s_pause_active = 0;
    s_pause_sprite_id = 0;
}

/* ======================================================================== */
/* PauseController_Update                                                   */
/* Per-frame pause controller update: checks START button for pause toggle. */
/* ======================================================================== */
void PauseController_Update(void)
{
    if (g_game_paused)
    {
        s_pause_active = 1;
    }
    else
    {
        s_pause_active = 0;
    }
}

/* ======================================================================== */
/* PauseController_Draw                                                     */
/* Draws the pause overlay sprite when paused.                              */
/* ======================================================================== */
void PauseController_Draw(void)
{
    if (!s_pause_active)
        return;

    /* Draw pause icon at center of screen */
    OAM_SetSprite(s_pause_sprite_id, 128, 80, 0x200, 0x0);
}

/* ======================================================================== */
/* Player_Init @ 0x0206b768+                                                */
/* Initializes player entity with starting position and default state.      */
/* Args: r0=entity container or NULL for default spawn                      */
/* ======================================================================== */
void Player_Init(void* entity)
{
    PhysicsPlayer* p;
    u32 start_x, start_y;

    if (entity == NULL)
    {
        /* Use default spawn point */
        start_x = g_respawn_x;
        start_y = g_respawn_y;
    }
    else
    {
        /* Extract position from entity data */
        start_x = *(u32*)((u8*)entity + 0x10);
        start_y = *(u32*)((u8*)entity + 0x14);
    }

    p = g_current_player;
    if (p == NULL)
        return;

    /* Initialize position */
    p->pos_x = start_x;
    p->pos_y = start_y;
    p->pos_z = 0;

    /* Initialize velocities */
    p->applied_vel_x = 0;
    p->applied_vel_y = 0;
    p->input_vel_x = 0;
    p->input_vel_y = 0;
    p->extra_vel = 0;

    /* Initialize direction (facing right) */
    p->direction = 1;

    /* Set initial state flags */
    p->state_flags = PLAYER_STATE_SPAWNING;
    p->control_flags = PLAYER_CTRL_PAUSABLE;

    /* Initialize gravity */
    p->gravity = 0x40;
    p->terminal_vel = 0x600;

    /* Initialize timers */
    p->action_timer = 0;
    p->slide_timer = 0;
    p->roll_timer = 0;
    p->dash_timer = 0;
    p->homing_timer = 0;
    p->wall_jump_timer = 0;
    p->invuln_timer = 0;
    p->trick_timer = 0;

    /* Clear pointers */
    p->object_binding = NULL;
    p->partner = NULL;
    p->partner_ref = NULL;
    p->action_data = NULL;
    p->homing_target = NULL;

    /* Clear action function pointers */
    p->action_fn = NULL;
    p->action_fn_2 = NULL;
    p->action_fn_3 = NULL;
    p->misc_fn_1 = NULL;
    p->misc_fn_2 = NULL;

    /* Initialize collision margins */
    p->margin_l = -8;
    p->margin_r = 8;
    p->margin_t = -16;
    p->margin_b = 16;

    /* Set spawn position */
    g_respawn_x = start_x;
    g_respawn_y = start_y;

    PauseController_Init();
}
