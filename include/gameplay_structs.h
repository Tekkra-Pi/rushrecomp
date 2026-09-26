/* Gameplay struct definitions for Sonic Rush decompilation.
 * Each struct is based on member-access analysis of source files. */

#ifndef GAMEPLAY_STRUCTS_H
#define GAMEPLAY_STRUCTS_H

#include "nds_types.h"

/* === Common Effect/Particle struct === */
typedef struct {
    s32 x;              /* +0x00 */
    s32 y;              /* +0x04 */
    s32 vel_x;          /* +0x08 */
    s32 vel_y;          /* +0x0C */
    u32 timer;          /* +0x10 */
    u32 active;         /* +0x14 */
    u32 type;           /* +0x18 */
} EffectParticle;

/* === Spark struct (extends EffectParticle) === */
typedef struct {
    s32 x;
    s32 y;
    s32 vel_x;
    s32 vel_y;
    u32 timer;
    u32 active;
} SparkEntry;

/* === Explosion struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 size;
    u32 frame;
    u32 timer;
    u32 active;
} ExplosionEntry;

/* === Impact struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 direction;
    u32 frame;
    u32 timer;
    u32 active;
} ImpactEntry;

/* === Debris struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 vx;
    s32 vy;
    u32 type;
    u32 timer;
    u32 active;
} DebrisEntry;

/* === Dust struct (shared by slidedusts, spindashdusts, wallsliedusts, landdusts) === */
typedef struct {
    s32 x;
    s32 y;
    s32 vel_x;
    s32 vel_y;
    u32 timer;
    u32 active;
} DustEntry;

/* === SmokePuff struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 vel_x;
    s32 vel_y;
    u32 timer;
    u32 active;
} SmokePuffEntry;

/* === HomingTrail struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 active;
} HomingTrailEntry;

/* === RollTrail struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 active;
} RollTrailEntry;

/* === TrickSparkle struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 active;
} TrickSparkleEntry;

/* === InvSparkle struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 active;
} InvSparkleEntry;

/* === SpeedLine struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 vel_x;
    u32 timer;
    u32 active;
} SpeedLineEntry;

/* === HomingTarget struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 active;
    u32 type;
} HomingTargetEntry;

/* === HUD Anim struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 sprite_id;
    u32 speed;
    u32 num_frames;
    u32 current_frame;
    u32 timer;
    u32 loop;
    u32 active;
} HudAnimEntry;

/* === Platform struct (shared by pathplats, moveplats, fallplats, crumbleplats, breakplats) === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 origin_x;
    s32 origin_y;
    s32 dx;
    s32 dy;
    s32 speed;
    u32 timer;
    u32 active;
} PlatformEntry;

/* === MovePlat-specific (extends PlatformEntry) === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 origin_x;
    s32 origin_y;
    s32 dx;
    s32 dy;
    s32 speed;
    u32 timer;
    u32 carrying;
    u32 active;
} MovePlatEntry;

/* === PathPlat struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 origin_x;
    s32 origin_y;
    s32 speed;
    u32 timer;
    s32 (*path)[2];
    u32 path_idx;
    u32 path_len;
    u32 active;
} PathPlatEntry;

/* === FallPlat struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 origin_y;
    s32 fall_vy;
    u32 timer;
    u32 falling;
    u32 fallen;
    u32 active;
} FallPlatEntry;

/* === CrumblePlat struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 timer;
    u32 respawn_timer;
    u32 crumbling;
    u32 gone;
    u32 active;
} CrumblePlatEntry;

/* === BreakPlat struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    u32 timer;
    u32 delay;
    u32 shaking;
    u32 broken;
    u32 active;
} BreakPlatEntry;

/* === BreakWall struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 broken;
    u32 active;
} BreakWallEntry;

/* === PushBlock struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 vx;
    s32 vy;
    u32 pushing;
    u32 active;
} PushBlockEntry;

/* === Enemy/Actor struct (shared by enemies, aiactors, enemyactors, actorrunners) === */
typedef struct {
    s32 x;
    s32 y;
    s32 vx;
    s32 vy;
    u32 dir;
    u32 type;
    u32 hp;
    u32 state;
    u32 timer;
    u32 active;
} EnemyActorEntry;

/* === Boss Actor struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 vx;
    s32 vy;
    u32 hp;
    u32 max_hp;
    u32 boss_type;
    u32 state;
    u32 timer;
    u32 active;
} BossActorEntry;

/* === Boss Laser struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 dir;
    u32 width;
    u32 duration;
    u32 timer;
    u32 active;
} BossLaserEntry;

/* === Boss Minion struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 dir;
    u32 hp;
    u32 state;
    u32 timer;
    u32 active;
} BossMinionEntry;

/* === Boss Projectile struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 vx;
    s32 vy;
    u32 type;
    u32 timer;
    u32 active;
} BossProjEntry;

/* === Boss Zone struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 boss_type;
    u32 triggered;
    u32 active;
} BossZoneEntry;

/* === Hazard struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 type;
    u32 active;
} HazardEntry;

/* === Spike struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 type;
    u32 dir;
    u32 count;
    u32 active;
} SpikeEntry;

/* === DeathPlane struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 active;
} DeathPlaneEntry;

/* === BouncePad struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 active;
} BouncePadEntry;

/* === SpringPad struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 force;
    u32 dir;
    u32 anim;
    u32 active;
} SpringPadEntry;

/* === DashPanel struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 speed;
    u32 dir;
    u32 active;
} DashPanelEntry;

/* === CheckpointWarp struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 active;
} CheckpointEntry;

/* === Ladder struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 active;
} LadderEntry;

/* === Vine struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 h;
    u32 active;
} VineEntry;

/* === InvBox struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 state;
    u32 active;
} InvBoxEntry;

/* === Capsule struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 active;
} CapsuleEntry;

/* === SignPost struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 active;
} SignPostEntry;

/* === GoaldPost struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 active;
} GoaldPostEntry;

/* === IceBreak struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 active;
} IceBreakEntry;

/* === StoneBreak struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 active;
} StoneBreakEntry;

/* === GlassBreak struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 active;
} GlassBreakEntry;

/* === PipeEntry struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 dest_x;
    u32 dest_y;
    u32 dest_zone;
    u32 active;
} PipeEntry;

/* === RespawnPoint struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 active;
} RespawnEntry;

/* === LevelFxs struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 param;
    u32 timer;
    u32 active;
} LevelFxEntry;

/* === BG Scroll struct === */
typedef struct {
    u32 bg_id;
    s32 x;
    s32 y;
    s32 offset_x;
    s32 offset_y;
    s32 speed_x;
    s32 speed_y;
    u32 active;
} BgScrollEntry;

/* === BG Anim struct === */
typedef struct {
    u32 bg_id;
    u32 tile_count;
    u32 speed;
    u32 timer;
    u32 frame;
    u32 active;
} BgAnimEntry;

/* === FG Deco struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 tile_id;
    u32 flags;
    u32 active;
} FgDecoEntry;

/* === TexScroll struct === */
typedef struct {
    u32 bg_id;
    u32 mode;
    s32 speed_x;
    s32 speed_y;
    s32 offset_x;
    s32 offset_y;
    u32 active;
} TexScrollEntry;

/* === Event Entry struct === */
typedef struct {
    u32 id;
    u32 type;
    u32 param;
    u32 active;
} EventEntry;

/* === ActionTrigger struct === */
typedef struct {
    u32 event_type;
    u32 param;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 triggered;
    u32 active;
} ActionTriggerEntry;

/* === LevelTrigger struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 type;
    u32 event_type;
    u32 param;
    u32 triggered;
    u32 active;
} LevelTriggerEntry;

/* === ExitTrigger struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 dest_zone;
    u32 dest_x;
    u32 dest_y;
    u32 triggered;
    u32 active;
} ExitTriggerEntry;

/* === ScrollLock struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 lock_type;
    u32 lock_val;
    u32 triggered;
    u32 active;
} ScrollLockEntry;

/* === CameraTrigger struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 mode;
    u32 triggered;
    u32 active;
} CameraTriggerEntry;

/* === Collectible struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 active;
} CollectibleEntry;

/* === RingGroup struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 spacing;
    u32 collected;
    u32 count;
    s32 positions[16];
    u32 active;
} RingGroupEntry;

/* === Enemy Death struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 active;
} EnemyDeathEntry;

/* === Enemy Shoot struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 dir;
    u32 type;
    u32 timer;
    u32 active;
} EnemyShootEntry;

/* === Enemy Projectile struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 vx;
    s32 vy;
    u32 type;
    u32 timer;
    u32 active;
} EnemyProjEntry;

/* === Boss AI struct === */
typedef struct {
    u32 boss_type;
    u32 boss_idx;
    u32 ai_type;
    u32 pattern;
    u32 state;
    u32 timer;
    u32 phase;
    u32 active;
} BossAiEntry;

/* === Boss Pattern struct === */
typedef struct {
    u32 type;
    u32 param;
    u32 timer;
    u32 active;
} BossPatternEntry;

/* === GrindRail segment === */
typedef struct {
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;
    u32 active;
} GrindRailSegEntry;

/* === WarpGate struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 dest_zone;
    u32 dest_x;
    u32 dest_y;
    u32 active;
} WarpGateEntry;

/* === MagZone struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 active;
} MagZoneEntry;

/* === Collision Pair struct === */
typedef struct {
    u32 obj_a;
    u32 obj_b;
    u32 active;
} CollisionPairEntry;

/* === Spatial Shape struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 type;
    u32 active;
} SpatialShapeEntry;

/* === ScreenWarp struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 dest_x;
    u32 dest_y;
    u32 active;
} ScreenWarpEntry;

/* === Popup struct === */
typedef struct {
    u32 state;
    u32 str_id;
    u32 timer;
    u32 active;
} PopupEntry;

/* === ItemBox struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 active;
} ItemBoxEntry;

/* === Switch struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 state;
    u32 active;
} SwitchEntry;

/* === DestroyBlock struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 active;
} DestroyBlockEntry;

/* === SwitchBlock struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 switch_id;
    u32 active;
} SwitchBlockEntry;

/* === Conveyor struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 speed;
    u32 active;
} ConveyorEntry;

/* === IceFloor struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    u32 active;
} IceFloorEntry;

/* === StickyWall struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 h;
    u32 active;
} StickyWallEntry;

/* === HammerThrower struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 dir;
    u32 timer;
    u32 active;
} FlamethrowerEntry;

/* === Bubble spawn struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 active;
} BubbleEntry;

/* === Lava struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 speed;
    u32 timer;
    u32 active;
} LavaEntry;

/* === CpuWrap struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 dest_x;
    u32 dest_y;
    u32 active;
} CpuWrapEntry;

/* === Trail FX struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 tile;
    u32 timer;
    u32 active;
} TrailFxEntry;

/* === Obj Behavior struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 obj_id;
    u32 behavior;
    u32 state;
    u32 timer;
    u32 active;
} ObjBehaviorEntry;

/* === Collision Dispatch struct === */
typedef struct {
    u32 type_a;
    u32 type_b;
    void (*callback)(void*);
    u32 active;
} CollDispatchEntry;

/* === Collision Resolve struct === */
typedef struct {
    u32 type;
    void (*callback)(void*);
    u32 active;
} CollResolveEntry;

/* === Obj List Manager struct === */
typedef struct {
    u32 list_type;
    u32 count;
    u32 active;
} ObjListMgrEntry;

/* === Overlay Zone struct === */
typedef struct {
    u32 zone_id;
    u32 active;
} OverlayZoneEntry;

/* === Special Entry struct === */
typedef struct {
    u32 type;
    s32 x;
    s32 y;
    u32 active;
} SpecialEntry;

/* === Anim struct === */
typedef struct {
    u32 tile_id;
    u32 num_frames;
    u32 speed;
    u32 timer;
    u32 frame;
    u32 active;
} AnimEntry;

/* === Menu Cursor struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 menu_id;
    u32 max_items;
    u32 selected;
    u32 active;
} MenuCursorEntry;

/* === Menu Text struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 str_id;
    u32 active;
} MenuTextEntry;

/* === Object Behavior Function type === */
typedef void* (*ObjBehaviorFn)(u32);

/* === Sprite Anim struct === */
typedef struct {
    u32 frames;
    u32 speed;
    u32 timer;
    u32 frame;
} SpriteAnimEntry;

/* === Sprite Frame struct === */
typedef struct {
    u32 tile_id;
    u32 attr;
} SpriteFrameEntry;

/* === Powerup struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 timer;
    u32 active;
} PowerupEntry;

/* === Particle struct (heap-allocated linked list) === */
typedef struct Particle {
    s32 x;
    s32 y;
    s32 vel_x;
    s32 vel_y;
    u32 type;
    u32 timer;
    u32 lifetime;
    struct Particle *next;
} Particle;

/* === Interrupt Handler Entry === */
typedef struct {
    u32 active;
    u32 id;
    void (*func)(void);
} InterruptHandlerEntry;

/* === Buffer struct === */
typedef struct {
    void *data;
    u32 size;
    u32 read_pos;
    u32 write_pos;
} BufferEntry;

/* === Stomper struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 origin_y;
    s32 speed;
    s32 range;
    u32 timer;
    u32 stomping;
    u32 active;
} StomperEntry;

/* === ActorFlyer struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 start_x;
    s32 start_y;
    s32 end_x;
    s32 end_y;
    s32 speed;
    s32 progress;
    u32 forward;
    u32 active;
} ActorFlyerEntry;

/* === ActorPatrol struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 origin_x;
    s32 origin_y;
    s32 speed;
    s32 range_x;
    s32 range_y;
    u32 timer;
    u32 active;
} ActorPatrolEntry;

/* === ActorRunner struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 vx;
    s32 vy;
    s32 speed;
    u32 dir;
    u32 type;
    u32 timer;
    u32 active;
} ActorRunnerEntry;

/* === ActorHover struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 origin_y;
    s32 hover_range;
    s32 range;
    s32 speed;
    u32 timer;
    u32 active;
} ActorHoverEntry;

/* === ActorOscillate struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 origin_x;
    s32 origin_y;
    s32 range;
    s32 speed;
    u32 axis;
    u32 timer;
    u32 active;
} ActorOscillateEntry;

/* === Enemy Pattern struct === */
typedef struct {
    u32 type;
    u32 count;
    u32 active;
} EnemyPatternEntry;

/* === Enemy Shoot struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 dir;
    u32 timer;
    u32 active;
} EnemyShootStruct;

/* === Affine Transform struct === */
typedef struct {
    s16 hdx;
    s16 hdy;
    s16 vdx;
    s16 vdy;
    s32 px;
    s32 py;
} AffineTransformEntry;

/* === Screen Transition struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 dest_x;
    u32 dest_y;
    u32 dest_zone;
    u32 active;
} ScreenTransEntry;

/* === Task Entry === */
typedef struct {
    u32 active;
    u32 id;
    void (*func)(void);
    u32 priority;
    u32 paused;
} TaskEntry;

/* === Callback Entry === */
typedef struct {
    u32 active;
    void (*func)(void);
    u32 type;
} CallbackEntry;

/* === DMA Channel Entry === */
typedef struct {
    u32 active;
    void (*callback)(void);
} DmaChannelEntry;

/* === Sound Channel Entry === */
typedef struct {
    u32 active;
    u32 id;
    u32 type;
    u32 sound_id;
    u32 volume;
    u32 pitch;
} SoundChannelEntry;

/* === Pool Entry === */
typedef struct {
    void *ptr;
    u32 size;
    u32 active;
} PoolEntry;

/* === HUD Draw Entry === */
typedef struct {
    u32 sprite_id;
    s32 x;
    s32 y;
    u32 frame;
    u32 flags;
} HUDDrawEntry;

/* === Menu Item Entry === */
typedef struct {
    const char *label;
    void (*callback)(void);
} MenuItemEntry;

/* === Hurtbox Helper Entry === */
typedef struct {
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    void *owner;
    u32 active;
} HurtboxHelperEntry;

/* === Chain Bonus Entry === */
typedef struct {
    u32 bonus;
    u32 timer;
    u32 active;
} ChainBonusEntry;

/* === Trail Point Entry === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
} TrailPointEntry;

/* === Ring Scatter Entry === */
typedef struct {
    s32 x;
    s32 y;
    s32 vx;
    s32 vy;
    u32 timer;
    u32 active;
} RingScatterEntry;

/* === Boss Trigger Entry === */
typedef struct {
    s32 x;
    s32 y;
    u32 boss_id;
    u32 triggered;
    u32 active;
} BossTriggerEntry;

/* === Pal Cycle Entry === */
typedef struct {
    u32 bg_id;
    u32 pal_bank;
    u32 speed;
    u32 len;
    u32 offset;
    u32 timer;
    u32 active;
} PalCycleEntry;

/* === Dash Actor Entry === */
typedef struct {
    s32 x;
    s32 y;
    s32 power;
    u32 dir;
    u32 active;
} DashActorEntry;

/* === Grind Actor Entry === */
typedef struct {
    const s32 *points;
    u32 num_points;
    u32 speed;
    u32 active;
} GrindActorEntry;

/* === Magnet Entry === */
typedef struct {
    s32 x;
    s32 y;
    u32 range;
    u32 strength;
    u32 active;
} MagnetEntry;

/* === Ring Collector Entry === */
typedef struct {
    s32 x;
    s32 y;
    u32 num_rings;
    u32 collected;
    u32 active;
} RingCollectorEntry;

/* === Sparkle FX Entry === */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 active;
} SparkleFxEntry;

/* === Event Sys Entry === */
typedef struct {
    u32 event_type;
    u32 param;
    u32 processed;
    u32 active;
} EventSysEntry;

/* === Action Sys Entry === */
typedef struct {
    u32 action_type;
    u32 param;
    u32 processed;
    u32 active;
} ActionSysEntry;

/* === GravZone struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 gravity;
    u32 active;
} GravZoneEntry;

/* === TimerZone struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 speed;
    u32 time_limit;
    u32 entered;
    u32 active;
} TimerZoneEntry;

/* === Whirlpool struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 radius;
    u32 strength;
    u32 angle;
    u32 active;
} WhirlpoolEntry;

/* === TrickBonus struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 trick_type;
    u32 timer;
    u32 active;
} TrickBonusEntry;

/* === Vine struct === */
typedef struct {
    s32 x;
    s32 y;
    s32 height;
    u32 active;
} VineEntryUpdated;

/* === Ladder Entry (updated) === */
typedef struct {
    s32 x;
    s32 y;
    s32 height;
    s32 w;
    u32 active;
} LadderEntryUpdated;

/* === ObjSpawner struct === */
typedef struct {
    s32 x;
    s32 y;
    u32 type;
    u32 range;
    u32 spawned;
    u32 active;
} ObjSpawnerEntry;

#endif /* GAMEPLAY_STRUCTS_H */
