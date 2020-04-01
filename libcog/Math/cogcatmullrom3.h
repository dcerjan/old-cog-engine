#ifndef _COG_CATMULLROM_3_H_
#define _COG_CATMULLROM_3_H_

#include "CogArray.h"
#include "CogVector3.h"
#include "cogtypes.h"

struct CogCatmullRom3 {
  CogArray(CogVector3)  points;
  CogArray(float)    arcs;
  float           len;
};
typedef struct CogCatmullRom3* CogCatmullRom3;

CogCatmullRom3   CogCatmullRom3Alloc    (void);
void          CogCatmullRom3AddPoint (CogCatmullRom3 self, const CogVector3 point);
void          CogCatmullRom3Clear    (CogCatmullRom3 self);
void          CogCatmullRom3Compile  (CogCatmullRom3 self, const int resolution, const Bool);

CogVector3       CogCatmullRom3Position (const CogCatmullRom3 self, const float t);
CogVector3       CogCatmullRom3Tangent  (const CogCatmullRom3 self, const float t, const float epsilon);

void          CogCatmullRom3Inspect  (const CogCatmullRom3 self);
void          CogCatmullRom3Free     (CogCatmullRom3 self);

#endif