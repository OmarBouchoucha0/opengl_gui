#ifndef BASE_MATH_H
#define BASE_MATH_H
#include "base_types.cpp"

#define Kilobytes(x) ((u64)(x) * 1024)
#define Megabytes(x) ((u64)(x) * 1024 * 1024)
#define Gigabytes(x) ((u64)(x) * 1024 * 1024 * 1024)
#define Terabytes(x) ((u64)(x) * 1024 * 1024 * 1024 * 1024)

#define PI 3.14159265359f
#define DegreeToRadian(x) ((f32)(x) * PI / 180.0f)
#define RadianToDegree(x) ((f32)(x) * 180.0f / PI)

struct V2 {
  i32 x;
  i32 y;
};

struct V2f {
  f32 x;
  f32 y;
};

struct Rectangle {
  f32 x;
  f32 y;
  f32 width;
  f32 height;
};

internal V2f rotatePointRadian(V2f offset, f32 theta);
internal V2f rotatePointDegree(V2f offset, f32 rotation);
internal f32 distanceBetweenTwoPoints(V2f p1, V2f p2);
internal f32 angleBetweenTwoPoints(V2f p1, V2f p2);

#endif // BASE_MATH_H
