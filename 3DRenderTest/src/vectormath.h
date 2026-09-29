#ifndef VECTORMATH_H
#define VECTORMATH_H

#include "raylib.h"
#include "typedefs.h"
#include <math.h>

#define ROTATE_Y_AXIS (Vector3){ 0.0f, 1.0f, 0.0f }

Vector3 GetDirection(Vector3 vector, Vector3 direction, Vector3 target);       // Get the difference between vector and its target
Vector3 GetDirectionFloat(Vector3 vector, Vector3 direction, Vector3 target); //Same as last but returns float
f32 AngleBetweenTwoVectors(Vector2 vector1, Vector2 vector2);                 // Get the dot product of 2 angles

#endif