#pragma once
#include "base_math.h"

internal V2f rotatePointRadian(V2f offset, f32 theta) {
  f32 c = cosf(theta);
  f32 s = sinf(theta);
  return {
      offset.x * c - offset.y * s,
      offset.x * s + offset.y * c,
  };
}

internal V2f rotatePointDegree(V2f offset, f32 rotation) {
  f32 theta = DegreeToRadian(rotation);
  f32 c = cosf(theta);
  f32 s = sinf(theta);
  return {
      offset.x * c - offset.y * s,
      offset.x * s + offset.y * c,
  };
}

internal f32 distanceBetweenTwoPoints(V2f p1, V2f p2) {
  return sqrtf(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2));
}

internal f32 angleBetweenTwoPoints(V2f p1, V2f p2) {
  return atan2f(p2.y - p1.y, p2.x - p1.x);
}
