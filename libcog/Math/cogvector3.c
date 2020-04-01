#include "CogVector3.h"

#include <math.h>
#include <stdio.h>
 
CogVector3 CogVector3Add(const CogVector3 v0, const CogVector3 v1) {
  return (CogVector3){
      .x = v0.x + v1.x, 
      .y = v0.y + v1.y, 
      .z = v0.z + v1.z  };
}

CogVector3 CogVector3Sub(const CogVector3 v0, const CogVector3 v1) {
  return (CogVector3) {
    .x = v0.x - v1.x, 
    .y = v0.y - v1.y, 
    .z = v0.z - v1.z
  };
}

CogVector3 CogVector3Cross(const CogVector3 v0, const CogVector3 v1) {
  return (CogVector3) {
    .x = v0.y * v1.z - v0.z * v1.y,
    .y = v0.z * v1.x - v0.x * v1.z,
    .z = v0.x * v1.y - v0.y * v1.x
  };
}

float CogVector3Dot(const CogVector3 v0, const CogVector3 v1) {
  return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}
 
CogVector3 CogVector3Lerp(const CogVector3 src, const CogVector3 dst, const float t) {
  return (CogVector3) { 
    .x = src.x + (dst.x - src.x) * t, 
    .y = src.y + (dst.y - src.y) * t,
    .z = src.z + (dst.z - src.z) * t 
  };
}

float CogVector3Len(const CogVector3 v) {
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

CogVector3 CogVector3Normalized(const CogVector3 v) {
  float l = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  return (CogVector3) {
    .x = v.x / l,
    .y = v.y / l,
    .z = v.z / l
  };
}

float CogVector3AngleBetween(const CogVector3 v0, const CogVector3 v1) {
  CogVector3 a,b;
  a = CogVector3Normalized(v0);
  b = CogVector3Normalized(v1);
  return acos(CogVector3Dot(a,b));
}

void CogVector3Inspect(const CogVector3 v) {
  printf("{\"x\": %f, \"y\": %f, \"z\": %f}", v.x, v.y, v.z);
}
