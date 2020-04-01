#ifndef _COG_GARBAGE_COLLECTOR_H_
#define _COG_GARBAGE_COLLECTOR_H_

#include "CogList.h"
#include "cogstring.h"

typedef void (*auto_destruct)(void*);
typedef struct CogGCEntry{
  auto_destruct   destructor;
  void*           object;
} CogGCEntry;
CogDefineList(CogGCEntry);

struct CogCleanupPool {
  CogString             name;
  CogList(CogGCEntry)   cleanupPool;
};
typedef struct CogCleanupPool* CogCleanupPool;

CogCleanupPool CogCleanupPoolAlloc(const char* name);
void CogCleanupPoolFree(CogCleanupPool self);
void CogCleanupPoolRegisterForCleanup(CogCleanupPool self, void* object, auto_destruct destructor);

#endif