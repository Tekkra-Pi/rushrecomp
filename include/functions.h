/* Cross-module function declarations.
 * Forward declarations for functions called across subsystems. */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "nds_types.h"
#include "entity.h"
#include "player.h"

/* === Entity === */
EntityContainer* Entity_AllocSlot(void);
EntityContainer* Entity_AllocContainer(
    void (*update_cb)(EntityContainer*),
    void (*destroy_cb)(EntityContainer*),
    u16 type, u16 marker, u16 data_size);
void Entity_Remove(EntityContainer *entity);
void Entity_WalkAll(EntityContainer *start, EntityContainer *terminator);
EntityContainer* Entity_GetHead(u32 list_id);
EntityContainer* Entity_GetPlayer(void);
void Entity_Init(void);

/* === Entity Pool === */
EntityContainer* EntityPool_Alloc(u32 type, u32 data_size);
void EntityPool_Free(EntityContainer *entity);
EntityContainer* EntityPool_FindByType(u32 type);
u32 EntityPool_FindAllByType(u32 type, EntityContainer **output, u32 max);

/* === Player === */
void Player_Update(void *player);
void Player_Destroy(void *player);
void Player_Draw(void);
void Player_SetPosition(s32 x, s32 y);
void Player_GetPosition(s32 *x, s32 *y);
void Player_SetVelocity(s32 vx, s32 vy);
void Player_GetVelocity(s32 *vx, s32 *vy);
s32 Player_GetVX(void);
s32 Player_GetVY(void);
u32 Player_GetSpeed(void);
u32 Player_IsAttacking(void);
u32 Player_IsFalling(void);
u32 Player_IsInvincible(void);
u32 Player_IsGrinding(void);
u32 Player_IsUnderwater(void);
void Player_SetInvincible(u32 val);
void Player_SetShield(u32 val);
void Player_SetSpeedShoes(u32 val);
void Player_SetUnderwater(u32 val);
void Player_SetGravity(s32 grav);
void Player_Hurt(void);
void Player_Kill(void);
void Player_AddRings(s32 count);
void Player_AddScore(u32 score);
void Player_AddLife(void);
void Player_LoseRings(s32 count);
void Player_AttractRings(s32 x, s32 y, u32 strength);
void Player_CollisionCheck(void);
u32 Player_GetState(PhysicsPlayer *player);
void Player_SetState(PhysicsPlayer *player, u32 new_state);
s32 Player_IsGrounded(PhysicsPlayer *player);
s32 Player_IsAlive(PhysicsPlayer *player);

/* === Movement === */
void Player_VelocityInjection(PhysicsPlayer *player);
void Player_MovementAccelHandler(PhysicsPlayer *player);
void Player_MovementCollisionResolution(PhysicsPlayer *player);
s32 Player_GroundCollisionSlopeSnap(PhysicsPlayer *player, s32 *slope_data);
void Player_CharController(PhysicsPlayer *player);
void Movement_Iterate(PhysicsPlayer *player);
void Action_Dispatch(PhysicsPlayer *player);
void Action_DecrementTimer(PhysicsPlayer *player);

/* === Homing === */
void HomingAttack_Init(PhysicsPlayer *player);
void HomingAttack_Update(PhysicsPlayer *player);
void HomingAttack_End(PhysicsPlayer *player);
void* HomingAttack_FindTarget(PhysicsPlayer *player);
void HomingAttack_HitTarget(PhysicsPlayer *player, void *target);

/* === Stomp === */
void Stomp_Init(PhysicsPlayer *player);
void Stomp_Update(PhysicsPlayer *player);
void Stomp_Bounce(PhysicsPlayer *player);
void Stomp_End(PhysicsPlayer *player);
void* Stomp_CheckEnemyCollision(PhysicsPlayer *player);

/* === Collision === */
s32 Collision_TestBox(s32 x, s32 y, s32 w, s32 h, s32 tx, s32 ty, s32 tw, s32 th);
s32 Collision_TestPoint(s32 x, s32 y, s32 tx, s32 ty, s32 tw, s32 th);
u32 Collision_GetResponseType(u32 obj_type);

/* === Object List === */
void ObjectList_Init(void);
void ObjectList_Update(void);
void ObjectList_Draw(void);
void ObjectList_Add(void *obj);
void ObjectList_Remove(void *obj);

/* === Object === */
void Object_Init(void);
void Object_Update(void);
void Object_Spawn(s32 x, s32 y, u32 type);
void Object_SetX(u32 id, s32 x);
void Object_SetY(u32 id, s32 y);
s32 Object_GetX(u32 id);
s32 Object_GetY(u32 id);
u32 Object_GetType(u32 id);
void Object_SetTile(u32 id, u32 tile);
void Object_SetRot(u32 id, u32 rot);

/* === Enemy === */
void Enemy_Init(void);
s32 Enemy_Spawn(s32 x, s32 y, u32 type, u32 dir);
u32 Enemy_GetHP(u32 type);
void Enemy_UpdateBehavior(u32 i);
void Enemy_Kill(u32 i);
void Enemy_Destroy(u32 i);

/* === Display === */
void Display_SetMasterBright(s32 val);
void Display_FillRect(s32 x, s32 y, s32 w, s32 h, u32 color);
void Display_FillRectAlpha(s32 x, s32 y, s32 w, s32 h, u32 color, u32 alpha);
void Display_DrawIris(s32 cx, s32 cy, s32 radius);
void Display_PrintFixed(s32 x, s32 y, const char *str);
void Display_SetFog(u32 color, u32 density);
void OAM_AddSprite(s32 x, s32 y, u32 tile, u32 attr);
void OAM_AddSpriteAffine(s32 x, s32 y, u32 tile, u32 attr, u32 rot);
void BG_SetScroll(u32 bg, u32 x, u32 y);
void WinSet(u32 win, s32 x1, s32 y1, s32 x2, s32 y2);

/* === Display Scroll === */
void DisplayScroll_Init(void);
void DisplayScroll_GetPosition(s32 *x, s32 *y);
void DisplayScroll_SetPosition(s32 x, s32 y);
void DisplayScroll_SetOffset(s32 ox, s32 oy);
void DisplayScroll_Update(void);
void DisplayScroll_SetBgOffset(u32 bg, s32 x, s32 y);

/* === Camera === */
void CameraBounds_Set(s32 left, s32 right, s32 top, s32 bottom);
void CameraBounds_SetLockX(s32 x);
void CameraBounds_SetLockY(s32 y);
void CameraShake_Start(s32 intensity, u32 duration);

/* === Camera Scroll/Lock/Pan/Zoom === */
void CamScroll_Start(s32 x, s32 y, u32 duration);
s32 CamScroll_GetX(void);
s32 CamScroll_GetY(void);
void CamLerp_SetTarget(s32 x, s32 y);
void CamLock_SetX(s32 x);
void CamLock_SetY(s32 y);

/* === Sound === */
void SoundEffect_Play(u32 id, u32 vol, u32 freq);
void Music_Trigger(u32 id);

/* === Input === */
u16 Input_GetPressed(void);
u16 Input_GetHeld(void);
u16 Input_GetReleased(void);

/* === Math === */
s32 Math_Sin(s32 angle);
s32 Math_Cos(s32 angle);
void Math_SinCos(u32 angle, s32 *sin_out, s32 *cos_out);
u32 Math_ArcTan2(s32 y, s32 x);
u32 Math_Sqrt(u32 val);
u32 Math_Rand(void);

/* === Memory === */
void *Mem_Alloc(u32 size);
void Mem_Free(void *ptr);
void Mem_Clear(void *ptr, u32 size);

/* === Heap === */
void Heap_Init(void);

/* === Timer === */
void Timer_Start(u32 duration);
void Timer_Stop(void);
void Timer_Update(void);

/* === Fade === */
void Fade_StartOut(void);
void Fade_StartIn(void);
s32 Fade_Update(void);

/* === Stage === */
void ChunkLoader_Update(void);
void ChunkLoader_Load(u32 zone, u32 act);
void Tilemap_DrawAll(void);
void Tilemap_Init(void);
void DispPrio_Sort(void);
void DispPrio_Draw(void);
void ObjListMgr_Draw(void);
void ObjListMgr_Update(void);

/* === Level === */
void LevelWarp_Start(u32 zone, s32 x, s32 y);
void LevelBounds_Set(s32 left, s32 right, s32 top, s32 bottom);
void LevelBoundsHelper_Init(void);
void LevelBoundsHelper_Update(void);
s32 LevelBounds_GetLeft(void);
s32 LevelBounds_GetRight(void);
s32 LevelBounds_GetTop(void);
s32 LevelBounds_GetBottom(void);

/* === Gameplay: Effects === */
void ExplosionFX_Spawn(s32 x, s32 y);
void Debris_Spawn(s32 x, s32 y, u32 type);

/* === Gameplay: Boss === */
s32 BossActor_Add(s32 x, s32 y, u32 boss_type);
void BossActor_Update(void);
s32 BossActor_GetHP(u32 i);
s32 BossActor_GetMaxHP(u32 i);
s32 BossActor_GetX(u32 i);
s32 BossActor_GetY(u32 i);
u32 BossActor_GetCount(void);
void BossHpBar_Start(u32 boss_idx);
void BossHpBar_Update(void);
void BossIntro_Start(void);
s32 BossIntro_IsDone(void);
void BossDefeat_Start(void);
void BossMinion_Add(s32 x, s32 y, u32 type);
void BossLaser_Add(s32 x, s32 y, u32 dir, u32 width, u32 duration);
void BossProj_Add(s32 x, s32 y, s32 vx, s32 vy, u32 type);
void Boss_UpdateAI(u32 i);
s32 Boss_GetHP(u32 type);

/* === Gameplay: Powerup === */
void PowerupHelper_Give(u32 type);

/* === Gameplay: Combo === */
void ComboCounter_Add(u32 count);
u32 ComboCounter_GetMax(void);

/* === Gameplay: Title Card === */
void TitleCard_Start(u32 zone, u32 act);
u32 TitleCard_IsDone(void);

/* === Gameplay: Result Tally === */
void ResultTally_Start(void);
s32 ResultTally_IsDone(void);

/* === Gameplay: HUD === */
void HudScoreIcon_Draw(void);
void HudRingIcon_Draw(void);
void HudTimerIcon_Draw(void);
void HudBoostIcon_Draw(void);
void HudLivesIcon_Draw(void);
void HudMgr_Init(void);
void HudMgr_Update(void);

/* === Gameplay: Mission Eval === */
void MissionEval_Init(void);
void MissionEval_Update(void);
s32 MissionEval_AllMet(void);

/* === Gameplay: Game === */
u32 Game_GetScore(void);
u32 Game_GetRings(void);
u32 Game_GetTime(void);

/* === Gameplay: Zone === */
void ZoneMgr_LoadZone(u32 zone, u32 act);
void ZoneMgr_SetZone(u32 zone, u32 act);

/* === Gameplay: Event/Action === */
void EventSys_Trigger(u32 event_type, u32 param);
void ActionSys_Trigger(u32 action_type, u32 param);

/* === Gameplay: Collect === */
u32 CollectHelper_GetCount(void);

/* === Gameplay: Spawn === */
s32 SpawnPoint_GetX(void);
s32 SpawnPoint_GetY(void);

/* === Gameplay: Parallax === */
void Parallax_Init(void);
void Parallax_Set(u32 bg, s32 factor_x, s32 factor_y);

/* === Gameplay: Level gravity/wind === */
void LevelGravity_Init(void);
void LevelGravity_Update(void);
void LevelWind_Init(void);
void LevelWind_Update(void);

/* === Save === */
void SaveMgr_Write(const char *key, u32 value);
u32 SaveMgr_Read(const char *key);

/* === String === */
int strcmp(const char *s1, const char *s2);
size_t strlen(const char *s);
char *strncpy(char *dst, const char *src, size_t n);
int sprintf(char *str, const char *fmt, ...);

/* === Touch === */
void Touch_Init(void);
void Touch_Update(void);

/* === Controller === */
void Controller_Init(void);
void Controller_Poll(void);

/* === NDS System === */
void System_Init(void);
void System_VBlankIntr(void);
void System_MainLoop(void);

#endif /* FUNCTIONS_H */