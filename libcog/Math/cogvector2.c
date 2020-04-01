#include <math.h>
#include <stdio.h>

#include "CogVector2.h"

CogVector2 CogVector2Add(const CogVector2 v0, const CogVector2 v1) {
  return (CogVector2){.x = v0.x + v1.x, .y = v0.y + v1.y};
}

CogVector2 CogVector2Sub(const CogVector2 v0, const CogVector2 v1) {
  return (CogVector2){.x = v0.x - v1.x, .y = v0.y - v1.y};
}

CogVector2 CogVector2Cross(const CogVector2 v) {
  return (CogVector2){.x = -v.y, .y = v.x};
}

CogVector2 CogVector2Normalized(const CogVector2 v) {
  float l = sqrt(v.x * v.x + v.y * v.y);
  return (CogVector2){v.x / l, v.y / l};
}

float CogVector2Len(const CogVector2 v) {
  return sqrt(v.x * v.x + v.y * v.y);
}

CogVector2 CogVector2Lerp(const CogVector2 src, const CogVector2 dst, const float t) {
  return (CogVector2){src.x + (dst.x - src.x) * t, src.y + (dst.y - src.y) * t};
}

float CogVector2Dot(const CogVector2 v0, const CogVector2 v1) {
  return v0.x * v1.x + v0.y * v1.y;
}

float CogVector2AngleBetween(const CogVector2 v0, const CogVector2 v1) {
  CogVector2 a = CogVector2Normalized(v0);
  CogVector2 b = CogVector2Normalized(v1);
  return acos(CogVector2Dot(a,b));
}

void CogVector2Inspect(const CogVector2 v) {
  printf("{\"x\": %f, \"y\": %f}", v.x, v.y);
}
