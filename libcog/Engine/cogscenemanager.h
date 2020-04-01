#ifndef _COG_SCENE_MANAGER_H_
#define _COG_SCENE_MANAGER_H_


#include "cogtypes.h"

struct CogScene;

void 
CogSceneManagerInit(
  void
);

struct CogScene* 
CogSceneManagerGetScene(
  const char* name
);

void
CogSceneManagerActivateScene(
  const char* name
);

void
CogSceneManagerRemoveScene(
  const char* name
);

void
CogSceneManagerAddScene(
  struct CogScene* scene
);

Bool
CogSceneManagerUpdate(
  const double dt
);

void
CogSceneManagerDraw(
  void
);

void
CogSceneManagerCleanUp(
  void
);

struct CogScene*
CogSceneManagerGetActiveScene(
  void
);


#endif