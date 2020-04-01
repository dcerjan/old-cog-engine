#include "cogvector4.h"

#include <math.h>
#include <stdio.h>

CogVector4 CogVector4Add(const CogVector4 v0, const CogVector4 v1) {
  return (CogVector4) {
    .x = v0.x + v1.x,
    .y = v0.y + v1.y,
    .z = v0.z + v1.z,
    .w = v0.w + v1.w
  };
}

CogVector4 CogVector4Sub(const CogVector4 v0, const CogVector4 v1) {
  return (CogVector4) {
    .x = v0.x - v1.x,
    .y = v0.y - v1.y,
    .z = v0.z - v1.z,
    .w = v0.w - v1.w
  };
}

float CogVector4Dot(const CogVector4 v0, const CogVector4 v1) {
  return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
}

float CogVector4Len(const CogVector4 v) {
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

CogVector4 CogVector4Normalized(const CogVector4 v) {
  float l = CogVector4Len(v);
  return (CogVector4){v.x / l, v.y / l, v.z / l, v.w / l};
}

CogVector4 CogVector4Lerp(const CogVector4 src, const CogVector4 dst, const float t) {
  return (CogVector4) { 
    .x = src.x + (dst.x - src.x) * t, 
    .y = src.y + (dst.y - src.y) * t,
    .z = src.z + (dst.z - src.z) * t,
    .w = src.w + (dst.w - src.w) * t 
  };
}

void CogVector4Inspect(const CogVector4 v) {
  printf("{\"x\": %f, \"y\": %f, \"z\": %f, \"w\": %f)>\n", v.x, v.y, v.z, v.w);
}
