#ifndef _COG_BEZIER_2_H_
#define _COG_BEZIER_2_H_

#include "CogArray.h"
#include "CogVector2.h"
#include "cogtypes.h"

struct CogBezier2 {
  CogArray(CogVector2)  points;
  float           len;
  Bool            loop;
};
typedef struct CogBezier2* CogBezier2;

CogBezier2   CogBezier2Alloc    (void);
void      CogBezier2AddPoint (CogBezier2 self, CogVector2 point);
void      CogBezier2Compile  (CogBezier2 self);

CogVector2   CogBezier2Position (CogBezier2 self, float t);
CogVector2   CogBezier2Tangent  (CogBezier2 self, float t);
CogVector2   CogBezier2Normal   (CogBezier2 self, float t);

void      CogBezier2Inspect  (CogBezier2 self);
void      CogBezier2Free     (CogBezier2 self);

#endif