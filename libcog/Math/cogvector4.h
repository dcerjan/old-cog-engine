#ifndef _COG_VECTOR_4_H_
#define _COG_VECTOR_4_H_

typedef struct CogVector4 {
  float x, y, z, w;
} CogVector4;

CogVector4 CogVector4Add(const CogVector4 v0, const CogVector4 v1);
CogVector4 CogVector4Sub(const CogVector4 v0, const CogVector4 v1);
CogVector4 CogVector4Lerp(const CogVector4 src, const CogVector4 dst, const float t);
CogVector4 CogVector4Normalized(const CogVector4 v);
float CogVector4Dot(const CogVector4 v0, const CogVector4 v1);
float CogVector4Len(const CogVector4 v);
void CogVector4Inspect(const CogVector4 v);

#endif