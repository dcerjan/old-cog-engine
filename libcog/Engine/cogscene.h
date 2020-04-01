#ifndef _COG_SCENE_H_
#define _COG_SCENE_H_


#include "cogtypes.h"
#include "cogstring.h"

struct CogSceneGraph;
struct CogCamera;
struct CogArray_CogLight;
struct CogHash_CogCamera;
struct CogScene {
  CogString                 name;
  
  struct CogSceneGraph*     sceneGraph;
  struct CogArray_CogLight* lights;
  struct CogHash_CogCamera* cameras;
  struct CogCamera*         activeCamera;
  
  Bool                      (*update)(struct CogScene*, double);
  Bool                      (*lateUpdate)(struct CogScene*, double);
  void                      (*draw)(struct CogScene*);
  void                      (*cleanup)(struct CogScene*);
  struct CogJsonObject*     (*serialize)(struct CogScene*);
  void                      (*deserialize)(struct CogScene*, struct CogJsonObject*);
};
typedef struct CogScene* CogScene;
CogDefineHash(CogScene);

void
CogSceneInitBase(
  CogScene self
);


#endif