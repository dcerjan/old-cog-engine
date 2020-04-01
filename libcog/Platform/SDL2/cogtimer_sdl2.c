#ifdef HAS_SDL2

#include <SDL2/SDL.h>
#include "cogtimer.h"

CogTimer CogTimerAlloc(const char* name) {
  CogTimer self = malloc(sizeof(struct CogTimer));
  
  self->elapsedTime = 0.0f;
  self->timeNew = 0.0f;
  self->timeOld = 0.0f;
  self->name = CogStringAlloc(name);
  self->paused = False;
  self->delta = 0.0f;
  
  return self;
}

void CogTimerFree(CogTimer self) {
  CogStringFree(self->name);
  free(self);
}

void CogTimerPause(CogTimer self) {
  self->paused = True;
}

void CogTimerResume(CogTimer self) {
  self->paused = False;
}

void CogTimerTick(CogTimer self) {
  self->timeOld = self->timeNew;
  self->timeNew = ((double)SDL_GetTicks()) / 1000.0;
  if(!self->paused) {
    self->delta = self->timeNew - self->timeOld;
    self->elapsedTime += self->delta;
  }
}

#endif