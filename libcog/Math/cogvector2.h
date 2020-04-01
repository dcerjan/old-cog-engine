#ifndef _COG_CogVector2_2_H_
#define _COG_CogVector2_2_H_

#include "CogArray.h"
#include "CogList.h"
#include "CogHash.h"

struct CogVector2 {
  float x,y;
};
typedef struct CogVector2 CogVector2;
CogDefineArray(CogVector2);
CogDefineList(CogVector2);
CogDefineHash(CogVector2);

CogVector2 CogVector2Add(const CogVector2 v0,const CogVector2 v1);
CogVector2 CogVector2Sub(const CogVector2 v0,const CogVector2 v1);
CogVector2 CogVector2Cross(const CogVector2 v);
CogVector2 CogVector2Normalized(const CogVector2 v);
CogVector2 CogVector2Lerp(const CogVector2 src, const CogVector2 dst, const float t);
float CogVector2Len(const CogVector2 v);
float CogVector2Dot(const CogVector2 v0,const CogVector2 v1);
float CogVector2AngleBetween(const CogVector2 v0,const CogVector2 v1);
void CogVector2Inspect(const CogVector2 v);

#endif