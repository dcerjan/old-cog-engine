#ifndef _COG_BOX_H_
#define _COG_BOX_H_

#include "CogVector3.h"

struct CogBox {
  CogVector3   min;
  CogVector3   max;
};
typedef struct CogBox CogBox;

#endif