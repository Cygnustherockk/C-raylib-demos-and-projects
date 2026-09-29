#include "raylib.h"
#include <tgmath.h>
#include <stdio.h>
#include "typedefs.h"

#ifndef ENEMY_H
#define ENEMY_H

typedef enum
{
  IDLE_STATE = 0,
  TRACKING_STATE = 1,
  ATTACKING_STATE = 2,
  DEAD_STATE = 3
} EnemyStates;

typedef struct Enemy
{
  Model enemyModel;
  BoundingBox enemyBoundingBox;
  BoundingBox enemyBoundingBoxPosition;
  Vector3 position;
  Vector3 targetPosition;
  f32 speed;
  f32 turnSpeed;
  f32 rotation;
  u8 state;
  Vector3 scale;
  Color color;
}Enemy;

typedef struct EnemyData
{
  u32 count;
  u64 size;
  Enemy *enemy;
}EnemyData;

Enemy InitEnemy(Camera3D camera, Enemy enemy, Model model);     // Set enemy params
Enemy MoveEnemy(Enemy enemy, Camera3D camera);                  // Move the enemy
Enemy DrawEnemy(Enemy enemy);                                   // Draw the enemy
u8 GetEnemyStates(Enemy enemy);                                 // Get enemy States
Enemy HandleEnemyStates(Enemy enemy, Camera3D camera);          // Handle enemy states
Enemy EnemyProcess(Enemy enemy, Camera3D camera);               // Main enemy Process in one nice function

#endif
