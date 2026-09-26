#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

void HomingAttack_Init(void)
{
    PhysicsPlayer* p = g_current_player;
    p->homing_timer = 60;
    p->homing_target = NULL;
    p->control_flags |= PLAYER_CTRL_BUSY;
}

void HomingAttack_Update(void)
{
    PhysicsPlayer* p = g_current_player;
    if (p->homing_timer > 0)
    {
        p->homing_timer--;
        if (p->homing_target != NULL)
        {
            s32 dx = p->pos_x - p->target_x;
            s32 dy = p->pos_y - p->target_y;
            if (dx < 0) dx = -dx;
            if (dy < 0) dy = -dy;
            if (dx < 0x1000 && dy < 0x1000)
            {
                p->applied_vel_x = (s16)(p->target_x - p->pos_x) >> 3;
                p->applied_vel_y = (s16)(p->target_y - p->pos_y) >> 3;
            }
        }
        else
        {
            p->applied_vel_y += p->gravity;
        }
    }
}

void HomingAttack_End(void)
{
    PhysicsPlayer* p = g_current_player;
    p->homing_timer = 0;
    p->homing_target = NULL;
}

void HomingAttack_HitTarget(void)
{
    PhysicsPlayer* p = g_current_player;
    p->homing_timer = 0;
    p->homing_target = NULL;
    p->applied_vel_x = 0;
    p->applied_vel_y = p->gravity;
    p->state_flags |= PLAYER_STATE_IMPACT;
}
