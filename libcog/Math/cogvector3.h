#ifndef _COG_VECTOR_3_H_
#define _COG_VECTOR_3_H_

#include "CogArray.h"
#include "CogList.h"
#include "CogHash.h"

typedef struct CogVector3 {
  float x,y,z;
} CogVector3;
CogDefineArray(CogVector3);
CogDefineList(CogVector3);
CogDefineHash(CogVector3);

CogVector3 CogVector3Add(const CogVector3 v0, const CogVector3 v1);
CogVector3 CogVector3Sub(const CogVector3 v0, const CogVector3 v1);
CogVector3 CogVector3Cross(const CogVector3 v0, const CogVector3 v1);
CogVector3 CogVector3Lerp(const CogVector3 src, const CogVector3 dst, const float t);
CogVector3 CogVector3Normalized(const CogVector3 v);
float CogVector3Len(const CogVector3 v);
float CogVector3Dot(const CogVector3 v0,const CogVector3 v1);
float CogVector3AngleBetween(const CogVector3 v0, const CogVector3 v1);
void CogVector3Inspect(const CogVector3 v);

#endif