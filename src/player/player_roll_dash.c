#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

void Roll_Init(void)
{
    PhysicsPlayer* p = g_current_player;
    p->roll_timer = 120;
    p->roll_direction = p->direction;
    p->control_flags |= PLAYER_CTRL_BUSY;
    p->state_flags |= PLAYER_STATE_T;
}

void Roll_Update(void)
{
    PhysicsPlayer* p = g_current_player;
    if (p->roll_timer > 0)
    {
        p->roll_timer--;
        if (p->roll_timer == 0)
        {
            Roll_End();
        }
    }
}

void Roll_End(void)
{
    PhysicsPlayer* p = g_current_player;
    p->roll_timer = 0;
    p->roll_direction = 0;
    p->control_flags &= ~PLAYER_CTRL_BUSY;
    p->state_flags &= ~PLAYER_STATE_T;
}

void Dash_Init(void)
{
    PhysicsPlayer* p = g_current_player;
    p->dash_timer = 30;
    p->dash_direction = p->direction;
    p->applied_vel_x = p->direction * 0x800;
    p->control_flags |= PLAYER_CTRL_BUSY;
    p->state_flags |= PLAYER_STATE_S;
}

void Dash_Update(void)
{
    PhysicsPlayer* p = g_current_player;
    if (p->dash_timer > 0)
    {
        p->dash_timer--;
        if (p->dash_timer == 0)
        {
            Dash_End();
        }
    }
}

void Dash_End(void)
{
    PhysicsPlayer* p = g_current_player;
    p->dash_timer = 0;
    p->dash_direction = 0;
    p->control_flags &= ~PLAYER_CTRL_BUSY;
    p->state_flags &= ~PLAYER_STATE_S;
}
