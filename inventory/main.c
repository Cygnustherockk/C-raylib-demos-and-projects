#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "src/parser.h"
#include "src/player.h"

#define BUFFER_SIZE 4096
#define SAMPLE_RATE 48000
#define GAME_ITEM ".gitm"

Vector2 lookRotation = { 0 };
float headTimer = 0.0f;
float walkLerp = 0.0f;
float headLerp = STAND_HEIGHT;
Vector2 lean = { 0 };

typedef struct ItemRegistry{
  ItemCollection* items;
  int count;
}ItemRegistry;

typedef struct InventoryItem{
  ItemCollection item;
  int amount;
  int max_amount;
}InventoryItem;

typedef struct Inventory{
  InventoryItem *items;
  int inventory_slots;
}Inventory;

FilePathList path_list;

ItemRegistry item_registry;

static Body player = {0};

int main(void)
{
  if(!DirectoryExists("items"))
  {
    printf("ERROR: No Item Path Exists, Adding Blank Item Path\n");
    MakeDirectory("items");
  }
  path_list = LoadDirectoryFiles("items");

  item_registry.items = malloc(sizeof(ItemCollection)*path_list.count);
  item_registry.count = path_list.count;

  for(unsigned int i = 0; i < path_list.count; i++)
  {
    if(!strcmp(GetFileExtension(path_list.paths[i]), GAME_ITEM)) //printf if there are NO differences
    {
      printf("Loading %s\n", path_list.paths[i]);
      item_registry.items[i] = GetFileData(path_list.paths[i]);
      item_registry.items[i].ID = i;

      for(int j = 0; j < item_registry.items[i].count; j++) printf("%s, %s\n", item_registry.items[i].item_data[j].item_name, item_registry.items[i].item_data[j].item_value);
    }
  }
  printf("Parsed %d Items\n", path_list.count);

  InitWindow(500, 500, "Game");
  SetTargetFPS(60);

  DisableCursor();

  Camera camera = { 0 };
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera.position = (Vector3)
  {
    player.position.x,
    player.position.y + (BOTTOM_HEIGHT + headLerp),
    player.position.z,
  };

  UpdateCameraFPS(&camera, &lookRotation, &lean, headTimer, walkLerp);

  while(!WindowShouldClose())
  {
    Vector2 mouseDelta = GetMouseDelta();
    lookRotation.x -= mouseDelta.x*sensitivity.x;
    lookRotation.y += mouseDelta.y*sensitivity.y;

    char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
    bool crouching = IsKeyDown(KEY_LEFT_CONTROL);
    UpdateBody(&player, lookRotation.x, sideway, forward, IsKeyPressed(KEY_SPACE), crouching);

    float delta = GetFrameTime();
    headLerp = Lerp(headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT), 20.0f*delta);
    camera.position = (Vector3){
        player.position.x,
        player.position.y + (BOTTOM_HEIGHT + headLerp),
        player.position.z,
    };

    if (player.is_on_floor && ((forward != 0) || (sideway != 0)))
    {
        headTimer += delta*3.0f;
        walkLerp = Lerp(walkLerp, 1.0f, 10.0f*delta);
        camera.fovy = Lerp(camera.fovy, 55.0f, 5.0f*delta);
    }
    else
    {
        walkLerp = Lerp(walkLerp, 0.0f, 10.0f*delta);
        camera.fovy = Lerp(camera.fovy, 60.0f, 5.0f*delta);
    }

    lean.x = Lerp(lean.x, sideway*0.02f, 10.0f*delta);
    lean.y = Lerp(lean.y, forward*0.015f, 10.0f*delta);

    UpdateCameraFPS(&camera, &lookRotation, &lean, headTimer, walkLerp);

    BeginDrawing();
    BeginMode3D(camera);
    ClearBackground(BLUE);
    DrawCubeV((Vector3){0, 0, 0}, (Vector3){10, 10, 10}, RED);
    EndMode3D();
    EndDrawing();
  }
  
  return 0;
}