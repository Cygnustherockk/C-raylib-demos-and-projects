#include "conwaysgame.h"

Pixel* InitScreen(Pixel* pixels)
{
  for(int i = 0; i < (WINDOW_SIZE/PIXEL_SIZE)*(WINDOW_SIZE/PIXEL_SIZE); i++)
  {
    int rand = GetRandomValue(1, 5);
    if(rand == 1){pixels[i].is_alive = true;}

    else{ pixels[i].is_alive = false;}
  }
  return pixels;
}


void DetermineFate(Pixel* pixels)
{
    for(int i = 0; i < (WINDOW_SIZE/PIXEL_SIZE)*(WINDOW_SIZE/PIXEL_SIZE); i++)
    {
      int n = GetNeighborCount(pixels, i);
      if(n < 2 AND pixels[i].is_alive) pixels[i].swap_next_frame = true; // Underpopulation
      else if(n > 3 AND pixels[i].is_alive) pixels[i].swap_next_frame = true; // Overpopulation
      else if(n==3 AND !pixels[i].is_alive) pixels[i].swap_next_frame = true; // Birth
      //if it's between 2 and 3 we ignore which essentially keeps it alive
    }
}


void SetNewState(Pixel* pixels)
{
      for(int i = 0; i < (WINDOW_SIZE/PIXEL_SIZE)*(WINDOW_SIZE/PIXEL_SIZE); i++)
    {
      if(pixels[i].swap_next_frame == true 
        AND pixels[i].is_alive == false)
        {pixels[i].is_alive = true; pixels[i].swap_next_frame = false;}

      if(pixels[i].swap_next_frame == true 
        AND pixels[i].is_alive == true)
        {pixels[i].is_alive = false; pixels[i].swap_next_frame = false;}
    }
}

int GetNeighborCount(Pixel* pixels, int pixel_loc)
{
  int neighbors = 0;

  if((pixel_loc >= 0 AND pixel_loc <= (WINDOW_SIZE/PIXEL_SIZE)) 
  OR (pixel_loc >= (WINDOW_SIZE/PIXEL_SIZE)*(WINDOW_SIZE/PIXEL_SIZE)-(WINDOW_SIZE/PIXEL_SIZE) AND pixel_loc <= (WINDOW_SIZE/PIXEL_SIZE)*(WINDOW_SIZE/PIXEL_SIZE)) 
  OR (pixel_loc % (WINDOW_SIZE/PIXEL_SIZE)) == 0
  OR (pixel_loc % (WINDOW_SIZE/PIXEL_SIZE)) == (WINDOW_SIZE/PIXEL_SIZE)-1)
  return neighbors; //Conways Border of Death (i dont want to deal with out of bounds pixels)

  if(pixels[pixel_loc+1].is_alive)                          neighbors++; // is right side alive ?
  if(pixels[pixel_loc-(WINDOW_SIZE/PIXEL_SIZE)].is_alive)   neighbors++; // is under alive ?
  if(pixels[pixel_loc-1].is_alive)                          neighbors++; // is left side alive ?
  if(pixels[pixel_loc+(WINDOW_SIZE/PIXEL_SIZE)].is_alive)   neighbors++; // is above alive ?
  if(pixels[pixel_loc+(WINDOW_SIZE/PIXEL_SIZE)-1].is_alive) neighbors++; // is above-left alive ?
  if(pixels[pixel_loc+(WINDOW_SIZE/PIXEL_SIZE)+1].is_alive) neighbors++; // is above-rigth alive ?
  if(pixels[pixel_loc-(WINDOW_SIZE/PIXEL_SIZE)-1].is_alive) neighbors++; // is under-left alive ?
  if(pixels[pixel_loc-(WINDOW_SIZE/PIXEL_SIZE)+1].is_alive) neighbors++; // is under-right alive ?

  return neighbors;
}

void DrawPixels(Pixel* pixels)
{
  int xbuf = 0;
  int ybuf = 0;
  for(int i = 0; i < (WINDOW_SIZE/PIXEL_SIZE)*(WINDOW_SIZE/PIXEL_SIZE); i++)
  { 
    if(xbuf == (WINDOW_SIZE/PIXEL_SIZE)){ xbuf = 0; ybuf++;}
    int xpos = PIXEL_SIZE*xbuf;
    int ypos = PIXEL_SIZE*ybuf;
    if(pixels[i].is_alive) DrawRectangle(xpos, ypos, PIXEL_SIZE, PIXEL_SIZE, WHITE);
    else DrawRectangle(xpos, ypos, PIXEL_SIZE, PIXEL_SIZE, BLACK);
    //if(pixels[i].swap_next_frame) DrawRectangle(xpos, ypos, PIXEL_SIZE, PIXEL_SIZE, RED);
    xbuf++;
  }
}