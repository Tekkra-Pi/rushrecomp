/* Global variable declarations for Sonic Rush decompilation.
 * All game-state globals used across subsystems. */

#ifndef GLOBALS_H
#define GLOBALS_H

#include "nds_types.h"

/* === Input === */
extern u16 g_key_current;
extern u16 g_key_previous;
extern u16 g_key_pressed;
extern u16 g_key_released;
extern u16 g_key_held;

/* === Touch === */
extern u16 g_touch_x;
extern u16 g_touch_y;
extern u16 g_touch_pressure;

/* g_touch_state defined in touch_state.h */

/* === Game State === */
extern u32 g_game_score;
extern u32 g_game_rings;
extern u32 g_game_time;
extern u32 g_game_lives;
extern u32 g_game_paused;
/* g_game_state defined in zone_state.h */
extern u32 g_gamestate_state;
extern u32 g_gamestate_prev;
extern u32 g_system_flags;
extern u32 g_current_zone;
extern u32 g_current_act;
extern u32 g_level_zone;
extern u32 g_level_act;
extern u32 g_lives;
extern u32 g_life_max;

/* === Player === */
extern u32 g_player_flags;
extern u32 g_player_state;
extern u32 g_player_timer;

/* === Camera === */
extern s32 g_camera_x;
extern s32 g_camera_y;
extern s32 g_camera_target_x;
extern s32 g_camera_target_y;
extern s32 g_respawn_x;
extern s32 g_respawn_y;

/* === Display === */
extern u32 g_display_layer_mask;
extern u32 g_window_layer_mask;
extern u32 g_window_width;
extern u32 g_window_height;
extern u32 g_window_x;
extern u32 g_window_y;
extern s32 g_target_scroll_x;
extern s32 g_target_scroll_y;

/* === Stage === */
extern s32 g_stage_width;
extern s32 g_stage_height;
extern u32 g_stage_load_progress;
extern u32 g_stageload_state;
extern u32 g_stage_tileset_id;
extern u32 g_stage_render_layer;
extern s32 g_stage_render_offset_x;
extern s32 g_stage_render_offset_y;
extern u32 g_stage_collision_width;
extern u32 g_stagecoll_active;
extern u32 g_stage_data_ptr;

/* === Sound === */
extern u32 g_sfx_volume;
extern u32 g_sfx_muted;
extern SoundChannelEntry g_sound_channels[];
extern u32 g_master_volume;
extern u32 g_music_volume;
extern u32 g_current_music;
extern u32 g_current_zone_music;
extern u32 g_music_fading;

/* === Fade === */
extern u32 g_fade_state;
extern u32 g_fade_timer;
extern u32 g_fadetoblack_state;

/* === Screen Transition === */
extern u32 g_screentrans_state;
extern u32 g_transition_type;
extern u32 g_transition_timer;
extern u32 g_transition_duration;
extern u32 g_disptrans_state;
extern u32 g_wipe_type;
extern u32 g_wipe_progress;

/* === HUD === */
extern u32 g_hud_update_flags;
extern u32 g_hud_update_timer;
extern u32 g_hudpos_active;
extern s32 g_hudpos_x;
extern s32 g_hudpos_y;
extern u32 g_hudscore_visible;
extern u32 g_hudrings_visible;
extern u32 g_hudtime_visible;
extern u32 g_hudboost_visible;
extern u32 g_hudlives_visible;
extern u32 g_hud_combo;
extern u32 g_hud_boost;
extern u32 g_hudscoreicon_y;
extern u32 g_hudlivesicon_y;
extern u32 g_hudtimericon_y;
extern u32 g_scoredisplay_visible;
extern u32 g_scoredisplay_score;
extern u32 g_scoredisplay_target;
extern s32 g_scoredisplay_x;
extern s32 g_scoredisplay_y;
extern u32 g_timerdisplay_visible;
extern s32 g_timerdisplay_x;
extern s32 g_timerdisplay_y;
extern u32 g_hudtrick_visible;
extern u32 g_hudtrick_name;
extern u32 g_hudtrick_timer;
extern s32 g_hudtrick_x;
extern s32 g_hudtrick_y;
extern u32 g_hudcombo_visible;
extern u32 g_hudboost_value;

/* === Score === */
extern u32 g_score;
extern u32 g_score_display;
extern u32 g_score_multiplier;
extern u32 g_scoremult_val;
extern u32 g_scoremult_max;
extern u32 g_scorepopup_count;
extern u32 g_ringbonus_score;
extern u32 g_ringbonus_state;
extern u32 g_ringbonus_timer;
extern u32 g_score_helper_bonus;

/* === Rings === */
extern u32 g_rings;
extern u32 g_rings_display;
extern RingScatterEntry g_ringscat[];
extern AnimEntry g_ringlayouts[];
extern RingCollectorEntry g_ringcolls[];
extern RingGroupEntry g_ringgroups[];
extern u32 g_ringgroup_count;
extern u32 g_ringhelper_count;
extern HudAnimEntry g_ringicons[];
extern u32 g_ringicon_count;

/* === Timer === */
extern u32 g_time;
extern u32 g_timer_ticks;
extern u32 g_timer_frames;
extern u32 g_timer_seconds;
extern u32 g_timer_minutes;
extern u32 g_timer_overflow;
extern u32 g_time_helper_elapsed;
extern u32 g_time_helper_warning;

/* === Boost === */
extern u32 g_boost_level;
extern u32 g_boost_max;
extern u32 g_boostlaunch_active;
extern u32 g_boostlaunch_dir;
extern s32 g_boostlaunch_x;
extern s32 g_boostlaunch_y;
extern u32 g_boostchain_count;
extern u32 g_boostchain_timer;
extern u32 g_boostaura_alpha;
extern u32 g_boostaura_size;
extern u32 g_boostfx_intensity;

/* === Speed Effect === */
extern u32 g_speedeffect_active;
extern u32 g_speedeffect_timer;
extern u32 g_speedlines_active;
extern u32 g_speedshoe_count;

/* === Camera Scroll/Lock/Pan/Zoom === */
extern s32 g_camscroll_timer;
extern u32 g_camlock_lock_x;
extern u32 g_camlock_lock_y;
extern u32 g_campan_active;
extern u32 g_campan_end_x;
extern u32 g_campan_end_y;
extern u32 g_camzoom_active;
extern u32 g_camzoom_level;
extern u32 g_canscroll_target_x;
extern u32 g_canscroll_target_y;
extern s32 g_canscroll_y;
extern u32 g_camfollow_deadzone_x;
extern u32 g_camfollow_deadzone_y;
extern u32 g_camfollow_speed_x;
extern u32 g_camfollow_speed_y;
extern u32 g_cammode_mode;
extern u32 g_camshake_active;
extern u32 g_camtrigger_count;
extern u32 g_zoomcam_speed;
extern u32 g_zoomcam_target;
extern u32 g_zoomcam_zoom;

/* === Fog === */
extern u32 g_fog_active;
extern u32 g_fog_color;

/* === Entities === */
extern u32 g_entity_count;
extern u32 g_entity_pool_size;
extern u32 g_pool_entries;
extern u32 g_pool_total_size;
extern u32 g_pool_used;

/* === Enemies === */
extern u32 g_enemy_count;
extern EnemyActorEntry g_enemies[];
extern CollectibleEntry g_enemspawns[];
extern EnemyActorEntry g_enemyactors[];
extern u32 g_enemyactor_count;
extern EnemyDeathEntry g_enemydeaths[];
extern EnemyPatternEntry g_enemypattern_count;
extern u32 g_enemyproj_count;
extern EnemyProjEntry g_enemyprojs[];
extern EnemyShootEntry g_enemyshoot_count;
extern EnemyShootEntry g_enemyshoots[];

/* === Boss === */
extern BossActorEntry g_bossactors[];
extern u32 g_bossactor_count;
extern BossAiEntry g_bossais[];
extern u32 g_bossai_count;
extern u32 g_bossdefeat_state;
extern u32 g_bosshpbar_active;
extern u32 g_bossintro_state;
extern u32 g_bosslaser_count;
extern BossMinionEntry g_bossminions[];
extern BossPatternEntry g_bosspattern_count;
extern u32 g_bossproj_count;
extern u32 g_bossshield_active;
extern u32 g_bossshield_hp;
extern BossTriggerEntry g_bosstrigs[];
extern BossZoneEntry g_bosszones[];
extern u32 g_bosszone_count;

/* === Object System === */
extern u32 g_objbehavior_count;
extern ObjBehaviorEntry g_objbehaviors[];
extern ObjListMgrEntry g_objlistmgrs[];
extern u32 g_objrender_count;
extern u32 g_objspawner_count;

/* === Action/Event System === */
extern ActionSysEntry g_actionsys[];
extern u32 g_actionsys_count;
extern u32 g_actiontrig_count;
extern EventEntry g_events[];
extern u32 g_eventsys_count;
extern EventSysEntry g_eventsyss[];
extern u32 g_evtflags[];
extern ActionTriggerEntry g_evttrigs[];
extern u32 g_exittrigger_count;
extern ExitTriggerEntry g_exittriggers[];
extern u32 g_stageevent_count;
extern EventEntry g_stage_events[];

/* === Level/Zone === */
extern u32 g_levelflag_count;
extern LevelTriggerEntry g_levtrigs[];
extern u32 g_levtrig_count;
extern ScrollLockEntry g_scrolllocks[];
extern u32 g_scrolllock_count;
extern u32 g_levelwarp_dest_zone;
extern s32 g_levelwarp_dest_x;
extern s32 g_levelwarp_dest_y;
extern u32 g_leveltrans_state;
extern u32 g_levelintro_state;
extern u32 g_leveloutro_state;
extern u32 g_cutscene_state;
extern u32 g_cutscene_timer;
extern u32 g_ending_state;

/* === Level Properties === */
extern u32 g_levelgravity_normal;
extern u32 g_levelwind_active;
extern u32 g_levelwind_strength;
extern u32 g_levelwater_active;
extern s32 g_levelwater_level;
extern u32 g_water_speed;

/* === Level Collision === */
extern u32 g_layer_collision_matrix[];
extern u32 g_collision_debug;
extern u32 g_collision_pairs;

/* === Object Lists === */
extern MovePlatEntry g_moveplats[];
extern u32 g_moveplat_count;
extern PushBlockEntry g_pushblocks[];
extern u32 g_pushblock_count;
extern BreakPlatEntry g_breakplats[];
extern BreakWallEntry g_breakwalls[];
extern DestroyBlockEntry g_destroyblocks[];
extern SwitchBlockEntry g_switchblocks[];
extern SwitchEntry g_switches[];
extern ItemBoxEntry g_itemboxes[];
extern ItemBoxEntry g_itemmonitors[];
extern CollectibleEntry g_itemspawns[];
extern RespawnEntry g_respawnpts[];
extern BouncePadEntry g_bouncepads[];
extern u32 g_bouncyfloor_count;
extern SpringPadEntry g_springpads[];
extern u32 g_springpad_count;
extern u32 g_springpad_force;
extern u32 g_bumperpad_count;
extern u32 g_bumperpad_force;
extern DashPanelEntry g_dashpanels[];
extern u32 g_dashpanel_count;
extern ConveyorEntry g_conveyor_count;
extern DeathPlaneEntry g_deathplanes[];
extern SpikeEntry g_spikes[];
extern u32 g_spike_count;
extern SpikeEntry g_spikeballs[];
extern HazardEntry g_hazards[];
extern FlamethrowerEntry g_flamethrower_count;
extern LavaEntry g_lavas[];
extern u32 g_lava_count;
extern BubbleEntry g_bubbles[];
extern GrindActorEntry g_grindactors[];
extern u32 g_grindrail_count;
extern GrindRailSegEntry g_railsegs[];
extern u32 g_grindind_active;
extern SparkEntry g_railgrindsparks[];
extern SparkEntry g_grindsparks[];

/* === Platforms/Motion === */
extern FallPlatEntry g_fallplats[];
extern u32 g_fallplat_count;
extern PathPlatEntry g_pathplats[];
extern u32 g_pathplat_count;
extern CpuWrapEntry g_cpwraps[];
extern u32 g_cpwrap_count;
extern SignPostEntry g_signposts[];
extern u32 g_signpost_count;
extern GoaldPostEntry g_goaldposts[];

/* === Invincibility/Shield === */
extern u32 g_invinv_timer;
extern u32 g_invinv_flicker;
extern u32 g_shield_type;
extern u32 g_shieldfx_active;
extern u32 g_shieldfx_timer;
extern u32 g_shieldbubble_active;
extern u32 g_shieldbubble_alpha;
extern u32 g_shieldbubble_timer;
extern InvSparkleEntry g_invsparkles[];

/* === Special Stage === */
extern u32 g_special_stage_state;

/* === Memory/Debug === */
extern u32 *g_stack_base;
extern u32 g_stack_pointer;
extern u32 g_stack_size;
extern DmaChannelEntry g_dma_channels[];
extern InterruptHandlerEntry g_interrupt_handlers[];
extern TaskEntry g_tasks[];
extern CallbackEntry g_callbacks[];
extern u32 g_sfxhelper_last;

/* === Misc Counters === */
extern u32 g_explosion_count;
extern u32 g_explosionfx_count;
extern ExplosionEntry g_explosionfxs[];
extern ExplosionEntry g_explosions[];
extern u32 g_debris_count;
extern DebrisEntry g_debris[];
extern SparkEntry g_sparks[];
extern SmokePuffEntry g_smokepuffs[];
extern DustEntry g_dusts[];
extern SmokePuffEntry g_dustclouds[];
extern DustEntry g_slidedusts[];
extern DustEntry g_spindashdusts[];
extern DustEntry g_wallsliedusts[];
extern DustEntry g_landdusts[];
extern RollTrailEntry g_rolltrails[];
extern SpeedLineEntry g_speedlines[];
extern HomingTargetEntry g_homingtargs[];
extern HomingTrailEntry g_homingtrails[];
extern u32 g_homingtarget_count;
extern InvBoxEntry g_invboxs[];
extern u32 g_invbox_count;
extern InvSparkleEntry g_invsparkles[];
extern CapsuleEntry g_capsules[];
extern u32 g_capsule_count;
extern TrickSparkleEntry g_tricksparkles[];
extern u32 g_trickbonus_count;
extern u32 g_trickchain_score;
extern u32 g_trickchain_scores[];
extern u32 g_trickchain_tricks[];
extern u32 g_trail_active;
extern u32 g_trail_entity_id;
extern u32 g_trailfx_count;
extern TrailFxEntry g_trailfxs[];
extern TrailPointEntry g_trail_points[];
extern u32 g_trail_type;
extern u32 g_trickui_active;
extern u32 g_trickui_score;
extern u32 g_trickui_timer;
extern u32 g_trickui_trick_id;
extern u32 g_tricknamedisp_name;
extern u32 g_tricknamedisp_timer;
extern u32 g_tricknamedisp_visible;
extern s32 g_tricknamedisp_x;
extern s32 g_tricknamedisp_y;
extern u32 g_springbounce_active;
extern u32 g_springbounce_timer;
extern s32 g_springbounce_x;
extern s32 g_springbounce_y;
extern u32 g_dasheffect_active;
extern u32 g_dasheffect_dir;
extern u32 g_dasheffect_timer;
extern s32 g_dasheffect_x;
extern s32 g_dasheffect_y;
extern u32 g_dashactors;
extern u32 g_hurtflash_alpha;
extern u32 g_hurtflash_timer;
extern u32 g_combo_active;
extern u32 g_combo_counter;
extern u32 g_combo_timer;
extern u32 g_combodisplay_visible;
extern s32 g_combodisplay_x;
extern s32 g_combodisplay_y;
extern ChainBonusEntry g_chainbonus[];
extern u32 g_chainbonus_count;
extern u32 g_chaindisplay_visible;
extern s32 g_chaindisplay_x;
extern s32 g_chaindisplay_y;
extern u32 g_chaindisp_timer;
extern u32 g_deathseq_timer;
extern u32 g_extra_life_pending;
extern u32 g_act_completed;
extern u32 g_act_rank;
extern u32 g_ladder_count;
extern LadderEntryUpdated g_ladders[];
extern u32 g_vine_count;
extern VineEntryUpdated g_vines[];
extern u32 g_spawncount;
extern u32 g_spawn_idx;
extern u32 g_chunkload_radius;
extern s32 g_chunkload_x;
extern s32 g_chunkload_y;

/* === Tile Anim === */
extern AnimEntry g_tileanims[];

/* === Display Effects === */
extern u32 g_palfade_active;
extern u32 g_palfade_timer;
extern u32 g_palfade_duration;
extern u32 g_palcycles;
extern u32 g_lighting_intensity;
extern u32 g_wateroverlay_active;
extern u32 g_watersurf_active;
extern u32 g_watersurf_amp;
extern u32 g_watersurf_speed;
extern s32 g_watersurf_y;
extern u32 g_dispeffect_count;
extern u32 g_dispwin_active;

/* === BG Display === */
extern u32 g_bgdisplay_visible;
extern s32 g_bgdisplay_x;
extern s32 g_bgdisplay_y;
extern BgAnimEntry g_bganims[];
extern BgScrollEntry g_bgscrolls[];
extern BgScrollEntry g_fgscrolls[];
extern u32 g_fgdeco_count;
extern FgDecoEntry g_fgdecos[];
extern u32 g_texscroll_count;
extern TexScrollEntry g_texscrolls[];
extern u32 g_dscroll_x;
extern u32 g_dscroll_y;
extern s32 g_dscroll_oy;
extern u32 g_dscroll_target_x;
extern u32 g_dscroll_target_y;
extern u32 g_parallax_y;
extern AnimEntry g_fganims[];
extern AnimEntry g_ringanims[];

/* === HUD Anim === */
extern HudAnimEntry g_hud_anims[];
extern u32 g_hud_anim_count;
extern HUDDrawEntry g_hud_draw_list[];

/* === Menu === */
extern u32 g_menu_index;
extern MenuItemEntry g_menu_items[];
extern MenuCursorEntry g_menucursors[];
extern u32 g_menucursor_count;
extern MenuTextEntry g_menutexts[];
extern s32 g_menuscroll_offset;
extern s32 g_menuscroll_speed;
extern u32 g_menubg_timer;

/* === Powerups === */
extern u32 g_powerup_count;
extern PowerupEntry g_powerups[];
extern u32 g_poweruphelper_active;
extern u32 g_poweruphelper_timer;
extern u32 g_poweruphelper_type;
extern u32 g_speedbooster_count;

/* === Particle === */
extern u32 g_sparklefxs;

/* === Cutscene/Dialog === */
extern u32 g_dialog_state;
extern u32 g_dialog_char_index;
extern u32 g_dialog_text;

/* === Mission === */
extern u32 g_missioneval_count;
extern u32 g_missioncomp_timer;
extern u32 g_missionfail_timer;
extern u32 g_missionfail_state;

/* === Results === */
extern u32 g_results_rank;
extern u32 g_results_rings;
extern u32 g_results_score;
extern u32 g_results_time;
extern u32 g_resulttally_state;
extern u32 g_resulttally_timer;
extern u32 g_totscore_score;

/* === Rank === */
extern u32 g_rank_helper_rank;
extern u32 g_rank_helper_rings;
extern u32 g_rank_helper_score;
extern u32 g_rank_helper_time;

/* === Best Record === */
extern u32 g_bestrecord_rings;
extern u32 g_bestrecord_score;
extern u32 g_bestrecord_time;
extern u32 g_besttime_time;
extern u32 g_besttime_timer;

/* === Level Save === */
extern u32 g_levelsave_rings;
extern u32 g_levelsave_score;

/* === Popup === */
extern u32 g_popup_state;
extern u32 g_popup_str;

/* === Zone Render === */
extern u32 g_overlayzone_count;
extern u32 g_tileanim_count;

/* === Path Follow === */
extern PipeEntry g_pipeentries[];
extern u32 g_pipeentry_count;
extern WarpGateEntry g_warpentries[];
extern u32 g_warpgate_count;

/* === Misc Gameplay === */
extern u32 g_gpound_timer;
extern s32 g_gpound_x;
extern s32 g_gpound_y;
extern u32 g_magnet_count;
extern MagnetEntry g_magnets[];
extern ActorFlyerEntry g_aiactors[];
extern ActorFlyerEntry g_actorflyers[];
extern u32 g_actorflyer_count;
extern ActorHoverEntry g_actorhovers[];
extern u32 g_actorhover_count;
extern ActorOscillateEntry g_actoroscillates[];
extern u32 g_actoroscillate_count;
extern ActorPatrolEntry g_actorpatrols[];
extern u32 g_actorpatrol_count;
extern ActorRunnerEntry g_actorrunners[];
extern u32 g_actorrunner_count;
extern StomperEntry g_actorstompers[];
extern u32 g_actorstomper_count;
extern CrumblePlatEntry g_crumbleplats[];
extern u32 g_crumbleplat_count;
extern u32 g_specialentry_count;
extern u32 g_msgdisp_str;
extern u32 g_msgdisp_timer;
extern u32 g_intro_step;

/* === Save === */
extern u32 g_savemgr_count;
extern u32 g_savemgr_dirty;
extern char g_savemgr_keys[64][32];
extern u32 g_savemgr_values[64];

/* === Collision Dispatch === */
extern CollDispatchEntry g_colldispatchs[];
extern CollResolveEntry g_collresolves[];
extern HurtboxHelperEntry g_hurtboxhs[];

/* === Spatial Grid === */
extern u32 g_spatial_grid[];
extern u32 g_spatial_grid_size;
extern SpatialShapeEntry g_spatial_shapes[];
extern u32 g_query_max;
extern u32 g_query_results;
extern u32 g_sweep_results;

/* === OAM === */
extern u32 g_oam_count;
extern u16 g_oam_buffer[];
extern u32 g_oamhelper_sprite_count;
extern SpriteAnimEntry g_spriteanim_count;
extern SpriteFrameEntry g_spriteframe_count;

/* === Cache === */
extern u32 g_cache_entries;
extern u32 g_cache_size;

/* === Queues === */
extern u32 g_queue_head;
extern u32 g_queue_tail;
extern u32 g_queue_items;

/* === Memory === */
extern BufferEntry g_buffers[];

/* === Misc === */
extern u32 g_effect_type;
extern u32 g_icyfloor_count;
extern u32 g_stickywall_count;
extern IceBreakEntry g_icebreaks[];
extern StoneBreakEntry g_stonebreaks[];
extern u32 g_glassbreak_count;
extern GlassBreakEntry g_glassbreaks[];
extern ImpactEntry g_impacts[];
extern u32 g_dmgfx_dir_x;
extern u32 g_dmgfx_dir_y;
extern u32 g_dmgfx_timer;
extern PopupEntry g_popups[];
extern ScreenWarpEntry g_screenwarps[];
extern u32 g_checkpoint_count;
extern u32 g_momentumzone_count;
extern u32 g_gravzone_count;
extern u32 g_timerzone_count;
extern u32 g_camcut_path;
extern u32 g_camcut_path_index;
extern u32 g_camcutscene_active;
extern s32 g_camdead_w;
extern s32 g_camdead_h;
extern u32 g_inputdisp_active;
extern u32 g_gradeeval_grade;

/* === Additional missing globals (cascade from other errors) === */
extern u32 g_fade_alpha;
extern u32 g_fog_density;
extern u32 g_lighting_mode;
extern u32 g_gamemode_player;
extern u32 g_modestate_mode;
extern u32 g_zone_flags;
extern u32 g_zonemgr_act;
extern u32 g_zonemgr_loaded;
extern u32 g_zonemgr_zone;
extern u32 g_levelbounds_left;
extern u32 g_levelbounds_right;
extern u32 g_levelbounds_top;
extern u32 g_levelbounds_bottom;
extern u32 g_cambounds_left;
extern u32 g_cambounds_right;
extern u32 g_cambounds_top;
extern u32 g_cambounds_bottom;
extern u32 g_camlock_x;
extern u32 g_camlock_y;
extern u32 g_grindind_x;
extern u32 g_grindind_y;
extern u32 g_titlecard_state;
extern u32 g_titlecard_timer;
extern u32 g_titlecard_alpha;
extern u32 g_titlecard_zone;
extern u32 g_titlecard_act;
extern u32 g_bubblespawn_timer;
extern u32 g_hud_lives;
extern u32 g_scoreicons;
extern u32 g_inventory;
extern u32 g_dispprio_list;
extern u32 g_dispwin_x1;
extern u32 g_dispwin_x2;
extern u32 g_dispwin_y1;
extern u32 g_dispwin_y2;
extern u32 g_skybox_scroll_x;
extern u32 g_skybox_scroll_y;
extern u32 g_skybox_tile;
extern s32 g_tilemap_parallax[];
extern s32 g_tilemap_parallax_y[];
extern u32 g_timebonus_value;
extern u32 g_playerattacker_x;
extern u32 g_playerattacker_y;
extern u32 g_playerphysics_max_vy;
extern GravZoneEntry g_gravzones[];
extern TimerZoneEntry g_timerzones[];
extern WhirlpoolEntry g_whirlpools[];
extern u32 g_whirlpool_count;
extern u32 g_levelevents;
extern u32 g_hazardhelpers;
extern u32 g_collectibles;
extern u32 g_collecthelpers;
extern u32 g_objrenders;
extern ObjSpawnerEntry g_objspawners[];
extern u32 g_musictiggers;
extern TrickBonusEntry g_trickbonus[];
extern u32 g_combo_max;
extern u32 g_bgscroll_count;
extern u32 g_fgscroll_count;
extern u32 g_railseg_count;
extern u32 g_scoremult_timer;
extern u32 g_combo_count;
extern u32 g_aiactor_count;
extern u32 g_springactor_count;
extern u32 g_bumperactor_count;
extern u32 g_objlistmgr_count;
extern u32 g_grindactor_count;

/* Forward-declared types - defined in their respective headers */
/* GameStateContext is defined in zone_state.h */
/* Particle is defined in its own header */
/* ObjectEntry is defined in object_list.h */
/* SpawnQueueState is a u32 enum */

#endif /* GLOBALS_H */
