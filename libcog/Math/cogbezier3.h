#ifndef _COG_BEZIER_3_H_
#define _COG_BEZIER_3_H_

#include "CogArray.h"
#include "CogVector3.h"
#include "cogtypes.h"

struct CogBezier3 {
  CogArray(CogVector3)  points;
  float           len;
  Bool            loop;
};
typedef struct CogBezier3* CogBezier3;

CogBezier3 CogBezier3Alloc(void);
void CogBezier3AddPoint(CogBezier3 self, CogVector3 point);
void CogBezier3Compile(CogBezier3 self);

CogVector3 CogBezier3Position(CogBezier3 self, float t);
CogVector3 CogBezier3Tangent(CogBezier3 self, float t);
CogVector3 CogBezier3Normal(CogBezier3 self, float t);

void CogBezier3Inspect(CogBezier3 self);
void CogBezier3Free(CogBezier3 self);

#endif