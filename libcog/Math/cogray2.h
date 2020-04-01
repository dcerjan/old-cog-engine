#ifndef _COG_RAY_2_H_
#define _COG_RAY_2_H_

#include "CogVector2.h"
#include "cogrect.h"
#include "cogcircle.h"

struct CogRay2 {
  CogVector2   start;
  CogVector2   direction;
};
typedef struct CogRay2 CogRay2;

#endif