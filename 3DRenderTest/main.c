#include "raylib.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "src/vectormath.h"
#include "src/typedefs.h"
#include "src/player.h"
#include "src/enemy.h"

int main(void) //Program entrypoint
{    
    // Window defaults
    const i32 screenWidth = 1600, screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "3D game test");

    // Basic enemy defaults
    Model model = LoadModel("models/penger.obj");
    Texture2D texture = LoadTexture("models/penger.png");

    Player player = InitPlayer(model, texture);
    Enemy enemy = InitEnemy(player.camera, enemy, model);

    
    enemy.enemyModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    // Set to 60 frames per second
    SetTargetFPS(60);    

    // Set window settings    
    ToggleBorderlessWindowed();

    // Main game loop
    while (!WindowShouldClose())// Detect window close button or ESC key
    { 

        if(NOT IsWindowFocused())// Actually minimize the window when tabbed out
        { 
          MinimizeWindow();
        }
        else
        {
          MaximizeWindow();
        }

        if(IsKeyDown(KEY_F11))
        {
          TakeScreenshot("screenshots/screenshot");
        }

        UpdateCamera(&player.camera, player.camera.projection);

        player.direction = GetDirectionVector(KEY_A, KEY_D, KEY_W, KEY_S); // Get the direction the player is facing

        Ray mousepos = GetScreenToWorldRay(GetMousePosition(), player.camera);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(player.camera);

        player.camera = MovePlayer(player.camera, player.direction); // Move the player
        enemy = EnemyProcess(enemy, player.camera); // Handle enemy
        DrawPlayer(&player, mousepos);
        DrawGrid(100, 1.0f); // Grid floor
        
        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadModel(model);
    UnloadTexture(texture);
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
