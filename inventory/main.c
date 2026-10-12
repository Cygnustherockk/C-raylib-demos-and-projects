#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>

#include "src/f_parser.h"
#include "src/p_player.h"
#include "terminal.h"

#define ENET_IMPLEMENTATION
#include "src/mp_client.h"


#define GAME_ITEM ".gitm"

#define TARGET_FPS 60

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

FilePathList path_list = {0};

ItemRegistry item_registry = {0};

static Camera camera = { 0 };

static Body player = {0};
static Vector2 sensitivity = { 0.01f, 0.01f };

static GameState game_state = Connecting;

static bool run_game = true;

void HandleLocalPlayerMovement()
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
}

void UpdateGame()
{
  EnetHandler(GetTime());
  switch(game_state)
  {
    case Disconnected:
      printf("Disconnected from server!\n");
      run_game = false;

    default: 
      break;
    
    case Connecting:
      printf("connecting...\n");
      if(Connected() == true && GetLocalPlayerID() >= 0){ game_state = Playing; printf("connected!\n");}
      break;

    case Disconnecting:
      printf("disconnecting\n");
      if(Connected() == false)
      {
        game_state = Disconnected;
      }
      break;

    case Playing:
      if(WindowShouldClose())
      {
        Disconnect();
        game_state = Disconnecting;
        break;
      }
      else if (Connected() == false)
      {
        InitEnetGameClient(HOST);
        game_state = Connecting;
        break;
      }
      else
      {
        HandleLocalPlayerMovement();

        UpdateLocalplayer(&player.velocity, GetFrameTime());
        break;
      }
  }
}

void* server_thread(void* arg) // IGNORE THE COMPILE WARN, PTHREAD NEEDS FUNCTIONS TO HAVE A VOID ARG TO WORK, OR ELSE IT CRASHES
{
  char server_run_command[256] = "server/server";
  system(server_run_command);
  return NULL;
}

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

  if(!DirectoryExists("server"))
  {
    printf("ERROR: No Server Path Found, Please Recompile Game To Fix This\n");
    return 1;
  }
  pthread_t thread;

  pthread_create(&thread, NULL, server_thread, NULL);

  InitWindow(500, 500, "Game");
  SetTargetFPS(TARGET_FPS);

  //DisableCursor();

  InitEnetGameClient(HOST);

  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera.position = (Vector3){player.position.x, player.position.y + (BOTTOM_HEIGHT + headLerp),player.position.z};

  UpdateCameraFPS(&camera, &lookRotation, &lean, headTimer, walkLerp);

  while(run_game)
  {
    BeginDrawing();
    BeginMode3D(camera);
    UpdateGame();
    ClearBackground(BLUE);
    DrawCubeV((Vector3){0, 0, 0}, (Vector3){10, 10, 10}, RED);
    for(int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
      Vector3 pos = {0};

      if(GetPlayerPos(i, &pos))
      {
        DrawCubeV(pos, (Vector3){5, 10, 5}, GREEN);
      }
    }

    EndMode3D();
    //HandleTerminal();
    EndDrawing();
  }
  StopEnet();
  CloseWindow();
  return 0;
}
