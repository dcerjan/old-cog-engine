#ifndef _COG_RAY_3_H_
#define _COG_RAY_3_H_

#include "CogVector3.h"
#include "cogbox.h"
#include "cogsphere.h"

struct CogRay3 {
  CogVector3   start;
  CogVector3   direction;
};
typedef struct CogRay3 CogRay3;

#endif