#ifndef _COG_MUTEX_H_
#define _COG_MUTEX_H_

#include "cogtypes.h"

struct CogMutex {
  CogString   name;
  uint32      id;
};
typedef struct CogMutex* CogMutex;
CogDefineArray(CogMutex);

CogMutex CogMutexAlloc(const char* name);
void CogMutexFree(CogMutex self);
int CogMutexLock(CogMutex self);
int CogMutexUnlock(CogMutex self);

#endif