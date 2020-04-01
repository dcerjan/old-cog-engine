#ifndef _COG_SPHERE_H_
#define _COG_SPHERE_H_

#include "CogVector3.h"

struct CogSphere {
  CogVector3   origin;
  float     radii;
};
typedef struct CogSphere CogSphere;

#endif