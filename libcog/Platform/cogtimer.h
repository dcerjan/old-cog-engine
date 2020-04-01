#ifndef _COG_TIMER_H_
#define _COG_TIMER_H_


#include "cogstring.h"

struct CogTimer {
  CogString   name;
  double      timeOld;
  double      timeNew;
  double      elapsedTime;
  double      delta;
  Bool        paused;
};
typedef struct CogTimer* CogTimer;

CogTimer CogTimerAlloc(const char* name);
void CogTimerFree(CogTimer self);
void CogTimerPause(CogTimer self);
void CogTimerResume(CogTimer self);
void CogTimerTick(CogTimer self);


#endif