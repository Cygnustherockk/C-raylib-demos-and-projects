#include "raylib.h"
#include "incl/rlgl.h"
#include "incl/raymath.h"
#include "frustum.h"

#include "perlin.h"

//This also ended up being a test on procedural generation, where a grid of lines would be drawn, there was also an attempt at using frustum culling (which was taken from the internet)
//It isnt super optimized, but I am very happy with the results given it was made from scratch, maybe ill edit the frustum logic to include render distance

Camera3D user;

#define WINDOW_REAL_SIZE 600


typedef struct GamePoint{
  bool can_draw;
  bool connected_bottom;
  bool connected_top;
  bool connected_left;
  bool connected_right;
  float perlinvalue;
}GamePoint;

typedef struct GamePointCollection{
  int index;
  GamePoint* point_values;
}GamePointCollection;


int main(void)
{
  InitWindow(WINDOW_REAL_SIZE, WINDOW_REAL_SIZE, "noise");
  SetTargetFPS(60);
  //ToggleFullscreen();
  //MaximizeWindow();

  float perlinvalues[PERLIN_SIZE*PERLIN_SIZE];//save this seperatly because game point will save height, and this will save the original value for later use (color)
  user.position = (Vector3){ 10.0f, 10.0f, 10.0f }; //Camera defaults
  user.target = (Vector3){ 0.0f, 1.0f, 0.0f };
  user.up = (Vector3){ 0.0f, 1.0f, 0.0f };    
  user.fovy = 45.0f;  
  user.projection = CAMERA_PERSPECTIVE;

  GamePointCollection point_collection;
  point_collection.point_values = malloc(sizeof(GamePoint)*PERLIN_SIZE*PERLIN_SIZE); //A game point should be around 8B, so size taken will be 8*PERLIN_NOISE^2

  Frustum frustum;

  
  DisableCursor();

  for(int y = 0; y < PERLIN_SIZE; y++)// repeat for x and y of perlin_size for each coordinate of a point in the noise map
  {
    for(int x = 0; x < PERLIN_SIZE; x++)
    {      
      float temp = 0; //reset variables because otherwise they save the changes and we dont want that
      int seed = 2;
      float amplitude = 1;
      float frequency = 0.01;
      perlinvalues[x+(y*PERLIN_SIZE)] = 0;
      for(int i = 0; i < 4; i++) //Brownian motion to make the noise smoother
      {  
        perlinvalues[x+(y*PERLIN_SIZE)] += amplitude * ((PerlinNoise2D(x*frequency, y*frequency, seed)-0.5*amplitude)*2*amplitude);

        amplitude*=0.5; //small changes for brownian
        frequency*=2;
        seed++;
      }

      temp = perlinvalues[x+(y*PERLIN_SIZE)];
      temp+= 1;
      temp/=2;

      float hmpos = (temp*25);

      point_collection.point_values[x+(y*PERLIN_SIZE)].perlinvalue = hmpos;

      point_collection.point_values[x+(y*PERLIN_SIZE)].can_draw = false; //Logic used later for drawing
      point_collection.point_values[x+(y*PERLIN_SIZE)].connected_bottom = false;
      point_collection.point_values[x+(y*PERLIN_SIZE)].connected_top = false;
      point_collection.point_values[x+(y*PERLIN_SIZE)].connected_left = false;
      point_collection.point_values[x+(y*PERLIN_SIZE)].connected_right = false;
      
    }
  }

  while(!WindowShouldClose())
  {
    UpdateCamera(&user, CAMERA_FREE);
    BeginDrawing();

    BeginMode3D(user);
    ClearBackground(BLACK);

    ExtractFrustum(&frustum);

    for(int y = 0; y < PERLIN_SIZE; y++)
    {
      for(int x = 0; x < PERLIN_SIZE; x++)
      {  

        if(PointInFrustumV(&frustum, (Vector3){ x, point_collection.point_values[x+(y*PERLIN_SIZE)].perlinvalue, y })) point_collection.point_values[x+(y*PERLIN_SIZE)].can_draw = true; //Is the point in the frustum ?
        
        else point_collection.point_values[x+(y*PERLIN_SIZE)].can_draw = false;

        point_collection.point_values[x+(y*PERLIN_SIZE)].connected_top = false; //Resset so they get drawn next frame
        point_collection.point_values[x+(y*PERLIN_SIZE)].connected_bottom = false;
        point_collection.point_values[x+(y*PERLIN_SIZE)].connected_left = false;
        point_collection.point_values[x+(y*PERLIN_SIZE)].connected_right = false;
        
      }
    }
    for(int y = 0; y < PERLIN_SIZE; y++) //Repeat on z axis
    {
      for(int x = 0; x < PERLIN_SIZE; x++) // repeat on x axis
      {
        if(point_collection.point_values[x+(y*PERLIN_SIZE)].can_draw) //We can draw this point
        {

        
          float temp = perlinvalues[x+(y*PERLIN_SIZE)];
          temp+=1;
          temp/=2;

          int c = (temp*127);
          c+=127;

          if(x > 0 ) //Avoid going out of bounds of the array
          {
            if(!point_collection.point_values[(x+(y*PERLIN_SIZE))-1].connected_right && !point_collection.point_values[(x+(y*PERLIN_SIZE))].connected_left) //Point at the left can be connected too
            {
              DrawLine3D((Vector3){ x, point_collection.point_values[x+(y*PERLIN_SIZE)].perlinvalue, y}, (Vector3){x-1, point_collection.point_values[(x+(y*PERLIN_SIZE))-1].perlinvalue, y}, (Color){0, c, 0, 255});
              point_collection.point_values[x+(y*PERLIN_SIZE)].connected_left = true;
              point_collection.point_values[x+(y*PERLIN_SIZE)-1].connected_right = true;
            }
          }
          if( x < PERLIN_SIZE-1) //-1 because indexes in arrays should be 1 less, so PERLIN_SIZE-1 should be the stopping point
          {
            if(!point_collection.point_values[(x+(y*PERLIN_SIZE))+1].connected_left && !point_collection.point_values[(x+(y*PERLIN_SIZE))].connected_right) //Point at the right can be connected
            {
              DrawLine3D((Vector3){ x, point_collection.point_values[x+(y*PERLIN_SIZE)].perlinvalue, y}, (Vector3){x+1, point_collection.point_values[(x+(y*PERLIN_SIZE))+1].perlinvalue, y}, (Color){0, c, 0, 255});
              point_collection.point_values[x+(y*PERLIN_SIZE)].connected_right = true;
              point_collection.point_values[x+(y*PERLIN_SIZE)+1].connected_left = true;
            }
          }
          if(y > 0)
          {
            if(!point_collection.point_values[(x+((y-1)*PERLIN_SIZE))].connected_bottom && !point_collection.point_values[(x+(y*PERLIN_SIZE))].connected_top)  //Point "under" (as in, higher z coordinate) can be connected
            {
              DrawLine3D((Vector3){ x, point_collection.point_values[x+(y*PERLIN_SIZE)].perlinvalue, y}, (Vector3){x, point_collection.point_values[(x+((y-1)*PERLIN_SIZE))].perlinvalue, y-1}, (Color){0, c, 0, 255});
              point_collection.point_values[x+(y*PERLIN_SIZE)].connected_top = true;
              point_collection.point_values[x+((y-1)*PERLIN_SIZE)].connected_bottom = true;
            }
          }
          if(y < PERLIN_SIZE-1)
          {
            if(!point_collection.point_values[(x+((y+1)*PERLIN_SIZE))].connected_top && !point_collection.point_values[(x+(y*PERLIN_SIZE))].connected_bottom) //Point "above" (as in, lower z coordinate) can be connected
            { 
              DrawLine3D((Vector3){ x, point_collection.point_values[x+(y*PERLIN_SIZE)].perlinvalue, y}, (Vector3){x, point_collection.point_values[(x+((y+1)*PERLIN_SIZE))].perlinvalue, y+1}, (Color){0, c, 0, 255});
              point_collection.point_values[x+(y*PERLIN_SIZE)].connected_bottom = true;
              point_collection.point_values[x+((y+1)*PERLIN_SIZE)].connected_top = true;
            }
          }
        }
      }
    }

    

    DrawGrid(10, 10);

    EndMode3D();
    EndDrawing();
  }
  free(point_collection.point_values);
  CloseWindow();
  return 0;
}