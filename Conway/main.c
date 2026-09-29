#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"
#include "conwaysgame.h"


int main(void)
{
  InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Conway");
  SetTargetFPS(FPS);
  Pixel* Pixel_collection = malloc(sizeof(Pixel)*(WINDOW_SIZE/PIXEL_SIZE)*(WINDOW_SIZE/PIXEL_SIZE));

  Pixel_collection = InitScreen(Pixel_collection);


  while(!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(BLUE);

    DetermineFate(Pixel_collection);
    DrawPixels(Pixel_collection);
    SetNewState(Pixel_collection);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}