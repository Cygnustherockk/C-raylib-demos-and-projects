#include "vectormath.h"
#include <stdio.h>

Vector3 GetDirection(Vector3 vector, Vector3 direction, Vector3 target)
{
  direction.x = (i32)target.x - (i32)vector.x; // Rounded to int to avoid very rapid fidgeting
  direction.y = (i32)target.y - (i32)vector.y;
  direction.z = (i32)target.z - (i32)vector.z;
  return direction;
}

Vector3 GetDirectionFloat(Vector3 vector, Vector3 direction, Vector3 target)
{
  direction.x = target.x - vector.x; // float needed for accuracy of dot product
  direction.y = target.y - vector.y;
  direction.z = target.z - vector.z;
  return direction; 
}

f32 AngleBetweenTwoVectors(Vector2 vector1, Vector2 vector2)
{
  f32 absolute = (vector1.x*vector2.x)+(vector1.y*vector2.y);
  f32 pythag = (sqrtf(vector1.x*vector1.x+vector1.y*vector1.y)*sqrtf(vector2.x*vector2.x+vector2.y*vector2.y));

  f32 dot = acos(absolute/pythag);
  dot *= 180/PI; // turn radians into degree  
  return dot;
}