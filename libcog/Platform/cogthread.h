#ifndef _COG_THREAD_H_
#define _COG_THREAD_H_


#include "cogstring.h"

struct CogThread {
  CogString   name;
  int         (*threadMain)(void*);
};
typedef struct CogThread* CogThread;
CogDefineArray(CogThread);

CogArray(CogThread) CogThreadGetAllThreads(void);
void CogThreadJoin(CogThread self);
void CogThreadJoinAllThreads(void);
CogThread CogThreadAlloc(const char* name, int (*func)(void*));
void CogThreadFree(CogThread* self);


#endif