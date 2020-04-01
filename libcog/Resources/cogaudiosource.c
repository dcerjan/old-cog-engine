#include "cogaudiosource.h"
#include "cogbundle.h"
#include "coglogger.h"
#include "cogplatform.h"

CogAudioSource CogAudioSourceAlloc(const char* name, struct CogBundle* bundle) {
  CogAudioSource self = malloc(sizeof(struct CogAudioSource));
  
  self->name = CogStringAlloc(name);
  
  if(bundle) {
    CogBundleAddAudioSource(bundle, self);
  }
  
  return self;
}

CogAudioSource CogAudioSourceAllocFromFile(const char* name, const char* filename, struct CogBundle* bundle) {
	CogAudioSource self = malloc(sizeof(struct CogAudioSource));
  
  self->name = CogStringAlloc(name);
  
  self->music = Mix_LoadMUS(filename);
  if(! self->music) {
    CogLoggerError(CogPlatformGetLogger(), "Unable to open audio file %s.\n", filename);
    exit(3);
  }
  
  if(bundle) {
    CogBundleAddAudioSource(bundle, self);
  }
  
  return self;
}

void CogAudioSourcePlay(CogAudioSource self) {
  if(self) {
    if(self->music) {
      Mix_PlayMusic(self->music, 0);
    }
  }
}

void CogAudioSourcePause(CogAudioSource self) {
  if(self) {
    if(self->music) {
      if(Mix_PlayingMusic()) {
        Mix_PauseMusic();
      }
    }
  }
}

void CogAudioSourceStop(CogAudioSource self) {
  if(self) {
    if(self->music) {
      if(Mix_PlayingMusic()) {
        Mix_PauseMusic();
        Mix_RewindMusic();
      }
    }
  }
}

void CogAudioSourceResume(CogAudioSource self) {
  if(self) {
    if(self->music) {
      Mix_PausedMusic();
      Mix_ResumeMusic();
    }
  }
}

void CogAudioSourceInspect(const CogAudioSource self) {
  
}

void CogAudioSourceFree(CogAudioSource self) {
  CogStringFree(self->name);
  free(self);
}
