#ifndef _CONWAYS_GAME_H_
#define _CONWAYS_GAME_H_

#ifndef RAYLIB_H
#include "raylib.h"
#endif

#define PIXEL_SIZE 4
#define WINDOW_SIZE 640
#define FPS 10

#define OR ||
#define AND &&

typedef struct Pixel{
  Rectangle rec;
  bool is_alive;
  bool swap_next_frame; //We do not want a pixel further down the chain to know the future state of its neighbors
}Pixel;

Pixel* InitScreen(Pixel* pixels);
void DetermineFate(Pixel* pixels);
void SetNewState(Pixel* pixels);
int GetNeighborCount(Pixel* pixels, int pixel_loc);
void DrawPixels(Pixel* pixels);


#endif