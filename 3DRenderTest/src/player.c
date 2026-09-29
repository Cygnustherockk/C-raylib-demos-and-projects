#include "player.h"
#include <stdio.h>

Vector3 GetDirectionVector(i32 positiveX, i32 negativeX, i32 positiveZ, i32 negativeZ) //get the direction the player == facing
{ 
  Vector3 direction = { 0.0f, 0.0f, 0.0f };

  //Forwards and Backwards
  if(IsKeyDown(positiveX))
  {
    direction.x += 10;    
  }
  if(IsKeyDown(negativeX))
  { 
    direction.x -= 10;
  }

  // Left and Right
  if(IsKeyDown(positiveZ))
  {
    direction.z += 10; 
  }
  if(IsKeyDown(negativeZ))
  {
    direction.z -= 10;
  }
  if(abs(direction.x) == abs(direction.z)){ // normalize vectors to make it so you don't run faster while strafing
    direction.x = direction.x / sqrt(2);
    direction.z = direction.z / sqrt(2);
  }
  return direction;
}

Camera3D MovePlayer(Camera3D camera, Vector3 direction)
{
  
  camera.position.x += direction.x * GetFrameTime();
  camera.position.z += direction.z * GetFrameTime();
  camera.target.x += direction.x * GetFrameTime();
  camera.target.z += direction.z * GetFrameTime();  

  return camera;
}

Player InitPlayer(Model model, Texture texture)
{
  Player player;
  player.camera.position = (Vector3){ 0.0f, 15.0f, -20.0f };  // Camera position
  player.camera.target = (Vector3){ 0.0f, 0.5f, 0.0f};        // Camera looking at position
  player.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };           // Camera up vector (rotation towards target)
  player.camera.fovy = 45.0f;                                 // Camera field-of-view Y
  player.camera.projection = CAMERA_FIRST_PERSON;             // Camera mode type
  player.model = model;
  player.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
  player.rotation = 0;
  player.scale = (Vector3){ 1.0, 1.0, 1.0 };

  return player;
}

f32 RotatePlayer(Player *player)
{
  f32 dot = player->direction.x*cos(player->rotation) + player->direction.z*sin(player->rotation);
  f32 det = player->direction.x * sin(player->rotation) - player->direction.z * cos(player->rotation);
  f32 angle = -atan2(det, dot) * 180/PI;
  printf("%f \n", angle);
  DrawRay((Ray){(Vector3)player->camera.target, (Vector3)player->direction}, RED);
  DrawRay((Ray){(Vector3)player->camera.target, (Vector3){cos(player->rotation), 0, sin(player->rotation)}}, BLUE);
  
  return angle;
}

void DrawPlayer(Player *player, Ray mousepos)
{

  player->rotation -= 180;
  if (IsKeyDown(KEY_W)){
    if(player->rotation > 180.0f){
      player->rotation -= 5.0f;
    }
    else if (player->rotation < 180.0f){
      player->rotation += 5.0f;
    }
  }
  if (IsKeyDown(KEY_A) AND NOT IsKeyDown(KEY_S)){
    if(player->rotation > 270.0f){
      player->rotation -= 5.0f;
    }
    else if (player->rotation < 270.0f){
      player->rotation += 5.0f;
    }
  }
  if (IsKeyDown(KEY_D)){
    if(player->rotation > 90.0f){
      player->rotation -= 5.0f;
    }
    else if (player->rotation < 90.0f){
      player->rotation += 5.0f;
    }
}  if (IsKeyDown(KEY_S) AND NOT IsKeyDown(KEY_A)){
    if(player->rotation > 0.0f){
      player->rotation -= 5.0f;
    }
    if(player->rotation >= 270.0f){
      player->rotation += 5.0f;
    }
    else if (player->rotation < 0.0f){
      player->rotation += 5.0f;
    }
  }

  if (IsKeyDown(KEY_W) AND IsKeyDown(KEY_A)){
    if(player->rotation > 225.0f){
      player->rotation -= 5.0f;
    }
    else if (player->rotation < 225.0f){
      player->rotation += 5.0f;
    }
  }
  if (IsKeyDown(KEY_W) AND IsKeyDown(KEY_D)){
    if(player->rotation > 135.0f){
      player->rotation -= 5.0f;
    }
    else if (player->rotation < 135.0f){
      player->rotation += 5.0f;
    }
  }

  if (IsKeyDown(KEY_S) AND IsKeyDown(KEY_A)){
    if(player->rotation > 315.0f){
      player->rotation -= 5.0f;
    }
    else if (player->rotation < 315.0f){
      player->rotation += 5.0f;
    }
  }
  
  if (IsKeyDown(KEY_S) AND IsKeyDown(KEY_D)){
    if(player->rotation > 45.0f){
      player->rotation -= 5.0f;
    }
    else if (player->rotation < 45.0f){
      player->rotation += 5.0f;
    }
  }

  
  player->rotation += 180;
  printf("%f \n", player->rotation);

  player->model.transform = MatrixRotateXYZ( (Vector3){ 0, DEG2RAD*player->rotation, 0  });
  DrawModel(player->model, player->camera.target, 1.0, WHITE);
  
}

