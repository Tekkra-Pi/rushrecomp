# Entity/Player System Topic

## Overview
The entity system manages game objects through a linked-list container architecture. The physics player is a specialized entity with velocity, collision, and action state handling.

## Entity Container (0x22b4574 linked list)

### Container Structure (0x1c bytes)
| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| +0x00 | 4 | prev | Previous link / list node |
| +0x04 | 4 | next | Next link (walker: r6 = [r6,4]) |
| +0x08 | 4 | update | Update function pointer |
| +0x0c | 4 | destroy | Destroy function pointer |
| +0x10 | 4 | data | DATA pointer (actual entity struct) |
| +0x14 | 2 | flags | Halfword flags (bit0 = skip in walk) |
| +0x16 | 2 | type | Halfword type (low 3 bits = priority, bit4 = always-update) |
| +0x18 | 2 | size | Halfword size/capacity |
| +0x1a | 2 | marker | Halfword marker (0xffff default) |

### Entity Functions
- **AllocContainer**: 0x020372d0 (396B) - Creates entity with update/destroy callbacks
- **WalkAll**: 0x02036e04 (144B) - Per-frame walker
- **Remove**: 0x020371c8 - Entity removal

### Special Container Types
- 0x0020, 0xEFFF, 0xF000 - Reserved for system entities

## Physics Player (0x134 bytes)

### Player Structure
| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| +0x00 | 4 | self | Self pointer / link |
| +0x04 | 4 | parent | Parent pointer |
| +0x08 | 4 | update | Update function pointer |
| +0x0c | 2 | flags | Halfword flags |
| +0x10 | 4 | pos_x | Position X (8:8 fixed) |
| +0x14 | 4 | pos_y | Position Y (8:8 fixed) |
| +0x18 | 4 | pos_z | Position Z (8:8 fixed) |
| +0x1c | 4 | target_x | Target X (movement iterator) |
| +0x20 | 4 | target_y | Target Y |
| +0x24 | 4 | target_z | Target Z |
| +0x28 | 2 | input_vel_x | Input velocity X |
| +0x2a | 2 | input_vel_y | Input velocity Y |
| +0x2c | 2 | input_vel_z | Input velocity Z |
| +0x2e | 2 | applied_vel_x | Applied velocity X |
| +0x30 | 2 | applied_vel_y | Applied velocity Y |
| +0x34 | 2 | extra_vel | Extra velocity (impact/spring) |
| +0x36 | 2 | raw_input_x | Raw input X (per-frame scratch) |
| +0x38 | 2 | raw_input_y | Raw input Y |
| +0x3a | 2 | raw_input_z | Raw input Z |
| +0x3c | 2 | slope_accel_x | Slope-accel source X |
| +0x3e | 2 | slope_accel_y | Slope-accel source Y |
| +0x40 | 2 | gravity | Gravity value |
| +0x42 | 2 | terminal_vel | Terminal velocity clamp |
| +0x48 | 2 | accel_clamp | Accel clamp (spring/transport) |
| +0x4a | 2 | hitbox_offset_x | AABB/hitbox offset X |
| +0x4c | 2 | hitbox_offset_y | AABB/hitbox offset Y |
| +0x4e | 2 | saved_vel_x | Saved velocity X |
| +0x50 | 2 | saved_vel_y | Saved velocity Y |
| +0x52 | 2 | saved_vel_z | Saved velocity Z |
| +0x54 | 2 | control_flags | Control flags (see below) |
| +0x58 | 4 | state_flags | State flags (see below) |
| +0x5c | 2 | status | Halfword status |
| +0x5e | 2 | prev_angle | Previous angle |
| +0x60 | 2 | prev_angle_stored | Prev angle stored by iterator |
| +0x62 | 2 | action_timer | Action timer |
| +0x64 | 1 | margin_l | Collision box margin L |
| +0x65 | 1 | margin_r | Margin R |
| +0x66 | 1 | margin_t | Margin T |
| +0x67 | 1 | margin_b | Margin B |
| +0x68 | 4 | action_fn | Action function pointer |
| +0x6c | 4 | action_fn_2 | Action function pointer 2 |
| +0x70 | 4 | action_fn_3 | Action function pointer 3 |
| +0x78 | 4 | misc_fn_1 | Misc function pointer |
| +0x7c | 4 | misc_fn_2 | Misc function pointer |
| +0x80 | 4 | object_binding | Object binding (springs/platforms) |
| +0x84 | 4 | partner | Partner/companion entity |
| +0x88 | 4 | partner_ref | Object pointer (partner ref) |
| +0x8c | 4 | action_data | Action data pointer |
| +0x94 | 4 | hitbox_group_a | Hitbox group A pointer |
| +0xa4 | 4 | hitbox_link_fn | Hitbox link function |
| +0xc0 | 4 | flag_word_c0 | Flag word |
| +0xd8 | 2 | state_halfword | Halfword state |
| +0xdc | 4 | hitbox_group_b | Hitbox group B pointer |
| +0x124 | 4 | action_state_a | Action state object A |
| +0x128 | 4 | action_state_b | Action state object B |
| +0x12c | 4 | action_state_c | Action state object C |
| +0x130 | 4 | mover_binding | Mover-controller binding |

### Control Flags (+0x54)
| Bit | Mask | Name | Description |
|-----|------|------|-------------|
| 0 | 0x01 | FACING | Facing direction |
| 2 | 0x04 | BUSY | Busy state |
| 5 | 0x20 | PAUSABLE | Pausable entity |
| 7 | 0x80 | R | Action timer enable |

### State Flags (+0x58)
| Bit | Mask | Name | Description |
|-----|------|------|-------------|
| 0 | 0x01 | GROUNDED | On ground |
| 2 | 0x04 | IMPACT | Impact state |
| 8 | 0x100 | SPAWNING | Spawning state |
| 13 | 0x2000 | FROZEN | Frozen state |
| 14 | 0x4000 | SOLID_GUARD | Solid guard |

## Player Functions
- **Spawn**: 0x0204da44 (348B) - Creates physics player entity
- **Update**: 0x0206c3d0 (812B) - Per-frame player update
- **Destroy**: 0x0206bef4 - Player destruction

## Pause/Menu Controller (0x77c bytes)
A separate entity type (NOT a second physics player) that wraps the player for pause/menu overlay handling.

- **Spawn**: 0x02055918 (1256B) - Creates pause controller
- **Update**: 0x020556a4 - Pause/menu frame update
- **Destroy**: 0x02055878 - Pause controller destruction

## Evidence Confidence
- **CONFIRMED-STATIC**: All struct layouts verified through static analysis
- **CONFIRMED-RUNTIME**: Player update flow verified through runtime testing

## References
- re-analysis.md lines 324-450
