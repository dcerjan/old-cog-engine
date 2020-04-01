#ifndef _COG_AUDIO_SOURCE_H_
#define _COG_AUDIO_SOURCE_H_

#include "cogstring.h"
#include "CogHash.h"
#include "SDL2/SDL_mixer.h"

struct CogBundle;
struct CogAudioSource {
  CogString      name;
  Mix_Music*     music;
};
typedef struct CogAudioSource* CogAudioSource;
CogDefineHash(CogAudioSource);

CogAudioSource CogAudioSourceAlloc(const char* name, struct CogBundle* bundle);
CogAudioSource CogAudioSourceAllocFromFile(const char* name, const char* filename, struct CogBundle* bundle);
void CogAudioSourcePlay(CogAudioSource self);
void CogAudioSourcePause(CogAudioSource self);
void CogAudioSourceStop(CogAudioSource self);
void CogAudioSourceResume(CogAudioSource self);
void CogAudioSourceInspect(const CogAudioSource self);
void CogAudioSourceFree(CogAudioSource self);

#endif