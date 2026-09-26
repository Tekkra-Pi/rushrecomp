#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

void Stomp_Init(void)
{
    PhysicsPlayer* p = g_current_player;
    p->stomp_bounce = 1;
    p->applied_vel_y = 0;
    p->gravity = 0;
    p->state_flags |= PLAYER_STATE_S;
    p->control_flags |= PLAYER_CTRL_BUSY;
}

void Stomp_Update(void)
{
    PhysicsPlayer* p = g_current_player;
    if (p->stomp_bounce)
    {
        p->applied_vel_y += 0x80;
        if (p->applied_vel_y > 0x600)
        {
            p->applied_vel_y = 0x600;
        }
    }
}

void Stomp_End(void)
{
    PhysicsPlayer* p = g_current_player;
    p->stomp_bounce = 0;
    p->gravity = 0x40;
    p->state_flags &= ~PLAYER_STATE_S;
}

void Stomp_Bounce(void)
{
    PhysicsPlayer* p = g_current_player;
    p->applied_vel_y = -0x700;
    p->stomp_bounce = 1;
}

void Stomp_BounceOffEnemy(void)
{
    PhysicsPlayer* p = g_current_player;
    p->applied_vel_y = -0x800;
    p->stomp_bounce = 1;
    p->state_flags |= PLAYER_STATE_IMPACT;
}
