#ifndef _COG_CATMULLROM_2_H_
#define _COG_CATMULLROM_2_H_

#include "CogArray.h"
#include "CogVector2.h"
#include "cogtypes.h"

struct CogCatmullRom2 {
  CogArray(CogVector2)  points;
  CogArray(float)    arcs;
  float           len;
};
typedef struct CogCatmullRom2* CogCatmullRom2;

CogCatmullRom2   CogCatmullRom2Alloc    (void);
void          CogCatmullRom2AddPoint (CogCatmullRom2 self, const CogVector2 point);
void          CogCatmullRom2Clear    (CogCatmullRom2 self);
void          CogCatmullRom2Compile  (CogCatmullRom2 self, const int resolution, const Bool);

CogVector2       CogCatmullRom2Position (const CogCatmullRom2 self, const float t);
CogVector2       CogCatmullRom2Tangent  (const CogCatmullRom2 self, const float t, const float epsilon);

void          CogCatmullRom2Inspect  (const CogCatmullRom2 self);
void          CogCatmullRom2Free     (CogCatmullRom2 self);

#endif