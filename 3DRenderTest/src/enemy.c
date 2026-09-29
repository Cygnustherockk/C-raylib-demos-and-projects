#include "enemy.h"

#ifndef VECTORMATH_H
#include "vectormath.h"
#endif

Enemy InitEnemy(Camera3D camera, Enemy enemy, Model model)
{
  enemy.enemyModel = model;
  enemy.position = (Vector3){ 10.0f, 10.0f, 10.0f};
  enemy.enemyBoundingBox = GetMeshBoundingBox(enemy.enemyModel.meshes[0]);
  enemy.enemyBoundingBoxPosition = enemy.enemyBoundingBox;
  enemy.targetPosition = camera.position;
  enemy.speed = 1.2f;
  enemy.rotation = 0.0f;
  enemy.turnSpeed = 10.0f;
  enemy.state = TRACKING_STATE;
  enemy.scale = (Vector3){ 1.0f, 1.0f, 1.0f};
  enemy.color = WHITE;
  return enemy;
}

BoundingBox UpdateEnemyBoundingBox(Enemy enemy)
{
  enemy.enemyBoundingBoxPosition.min.x = enemy.position.x + enemy.enemyBoundingBox.min.x; // I really wish there was an easier way to do this
  enemy.enemyBoundingBoxPosition.min.y = enemy.position.y + enemy.enemyBoundingBox.min.y;
  enemy.enemyBoundingBoxPosition.min.z = enemy.position.z + enemy.enemyBoundingBox.min.z;
  enemy.enemyBoundingBoxPosition.max.x = enemy.position.x + enemy.enemyBoundingBox.max.x;
  enemy.enemyBoundingBoxPosition.max.y = enemy.position.y + enemy.enemyBoundingBox.max.y;
  enemy.enemyBoundingBoxPosition.max.z = enemy.position.z + enemy.enemyBoundingBox.max.z;
  return enemy.enemyBoundingBoxPosition;
}

Enemy MoveEnemy(Enemy enemy, Camera3D camera)
{
  Vector3 direction = GetDirection(enemy.position, direction, enemy.targetPosition);

  if(direction.x != 0){enemy.position.x += (direction.x/fabs(direction.x)) * enemy.speed * GetFrameTime();} // If direction != 0 (in front of the player), move the enemy
  if(direction.y != 0){enemy.position.y += (direction.y/fabs(direction.y)) * enemy.speed * GetFrameTime();}
  if(direction.z != 0){enemy.position.z += (direction.z/fabs(direction.z)) * enemy.speed * GetFrameTime();}

  enemy.enemyBoundingBoxPosition = UpdateEnemyBoundingBox(enemy);
  
  return enemy;
}

Enemy DrawEnemy(Enemy enemy)
{
  if(enemy.state != DEAD_STATE)
  {
    Vector3 directionVector = GetDirectionFloat(enemy.position, directionVector, enemy.targetPosition);
    Vector2 forward = { sinf(0), cosf(0) };
    enemy.rotation = AngleBetweenTwoVectors((Vector2){directionVector.x, directionVector.z}, forward);

    if(directionVector.x <= 0) enemy.rotation *= -1; // Reverse if player is "behind" enemy as dot product only covers 180 degrees

    DrawModelEx(enemy.enemyModel, enemy.position, ROTATE_Y_AXIS, enemy.rotation, enemy.scale, enemy.color);
  }
  return enemy;
}

u8 GetEnemyStates(Enemy enemy)
{
  Vector3 direction = GetDirection(enemy.position, direction, enemy.targetPosition);
  u32 distance = fabs(direction.x) + fabs(direction.y) + fabs(direction.z);

  if(distance > 50) //If distance is further than 50 units, don't track
  { 
    enemy.state = IDLE_STATE;
  }

  else if(distance <= 50 AND distance > 1) //If between 50 units and 1 unit, follow player
  { 
    enemy.state = TRACKING_STATE;
  }
  else if (distance <= 1) //If at 1 unit, attack enemy
  { 
    enemy.state = ATTACKING_STATE;
  }
  return enemy.state;
}

Enemy HandleEnemyStates(Enemy enemy, Camera3D camera)
{
  enemy.targetPosition = camera.target; // Update target position as otherwise it gets stuck

  
  if(enemy.state == IDLE_STATE)
  {
  }
  else if (enemy.state == TRACKING_STATE)
  {
    Vector3 directionVector = GetDirection(enemy.position, directionVector, enemy.targetPosition);
    enemy = MoveEnemy(enemy, camera);
  }
  
  return enemy;
}

Enemy DamageEnemy(Enemy enemy, Camera3D camera)
{
  Ray ray;
  RayCollision collision = { 0.0f };
  collision.distance = INT_FAST32_MAX;
  ray = GetScreenToWorldRay(GetMousePosition(), camera);
  collision = GetRayCollisionBox(ray, enemy.enemyBoundingBoxPosition);
  if (collision.hit)
  {
    if(IsKeyPressed(KEY_C))
    {
      enemy.state = DEAD_STATE;
    }
  }
  else
  { 
    collision.hit = false; 
  }

  return enemy;
}

Enemy EnemyProcess(Enemy enemy, Camera3D camera)
{ 
  if(NOT(enemy.state == DEAD_STATE))
  {
    enemy.state = GetEnemyStates(enemy);
    enemy = HandleEnemyStates(enemy, camera);
    enemy = DamageEnemy(enemy, camera);
    enemy = DrawEnemy(enemy);
  }
  return enemy;
}