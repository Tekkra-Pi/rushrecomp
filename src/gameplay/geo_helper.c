/* Gameplay level geometry helper functions. */
#include "nds_types.h"

s32 GeoHelper_GetFloorY(void) { s32 px, py; Player_GetPosition(&px, &py); return StageCollision_GetFloor(px, py); }
s32 GeoHelper_GetCeilingY(void) { s32 px, py; Player_GetPosition(&px, &py); return StageCollision_GetCeiling(px, py); }
s32 GeoHelper_GetWallRight(void) { s32 px, py; Player_GetPosition(&px, &py); return StageCollision_GetWallRight(px, py); }
s32 GeoHelper_GetWallLeft(void) { s32 px, py; Player_GetPosition(&px, &py); return StageCollision_GetWallLeft(px, py); }
s32 GeoHelper_IsOnGround(void) { return Player_IsOnGround(); }
s32 GeoHelper_IsUnderCeiling(void) { return Player_IsUnderCeiling(); }
s32 GeoHelper_IsTouchingWallLeft(void) { return Player_IsTouchingWallLeft(); }
s32 GeoHelper_IsTouchingWallRight(void) { return Player_IsTouchingWallRight(); }
s32 GeoHelper_GetSlopeAngle(void) { return Player_GetSlopeAngle(); }
s32 GeoHelper_IsFlat(void) { return Player_IsOnGround() && Player_GetSlopeAngle() == 0 ? 1 : 0; }
