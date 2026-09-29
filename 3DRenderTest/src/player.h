#include "raylib.h"
#include "../include/raymath.h"
#include "typedefs.h"
#include <math.h>
#include "vectormath.h"

#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player
{
  Camera3D camera;
  Model model;
  Mesh mesh;
  Texture texture;
  Vector3 position;
  f32 rotation;
  Vector3 direction;
  Ray forward;
  Vector3 scale;
}Player;

Vector3 GetDirectionVector(i32 positiveX, i32 negativeX, i32 positiveZ, i32 negativeZ); // Get the direction the player is facing
Camera3D MovePlayer(Camera3D camera, Vector3 direction);                                // Move the player based on direction
Player InitPlayer(Model model, Texture texture);                                                   // Set player params
void DrawPlayer(Player *player, Ray mousepos);                                                       // Draw Player
#endif