#ifndef _COG_CIRCLE_H_
#define _COG_CIRCLE_H_

#include "CogVector2.h"

struct CogCircle {
  CogVector2   origin;
  float     radii;
};
typedef struct CogCircle CogCircle;

#endif