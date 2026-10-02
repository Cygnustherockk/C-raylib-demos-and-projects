#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "raylib.h"
#include "Third-Party/raymath.h"

#define GRAVITY         32.0f
#define MAX_SPEED       20.0f
#define CROUCH_SPEED     5.0f
#define JUMP_FORCE      12.0f
#define MAX_ACCEL      150.0f
// Grounded drag
#define FRICTION         0.86f
// Increasing air drag, increases strafing speed
#define AIR_DRAG         0.98f
// Responsiveness for turning movement direction to looked direction
#define CONTROL         15.0f
#define CROUCH_HEIGHT    0.0f
#define STAND_HEIGHT     1.0f
#define BOTTOM_HEIGHT    0.5f

#define NORMALIZE_INPUT  0

typedef struct Body{
  Vector3 position;
  Vector3 velocity;
  Vector3 dir;
  bool is_on_floor;
}Body;

static Vector2 sensitivity = { 0.1f, 0.1f };

void UpdateCameraFPS(Camera *camera, Vector2 *lookRotation, Vector2 *lean, float headTimer, float walkLerp);
void UpdateBody(Body *body, float rot, char side, char forward, bool jumpPressed, bool crouchHold);

#endif