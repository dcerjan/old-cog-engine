#include "cogscene.h"
#include "cogcamera.h"
#include "coglight.h"
#include "cogscenegraph.h"
#include "cognode.h"
#include "CogJsonObject.h"

void CogSceneInitBase(CogScene self) {
  self->cameras = CogHashAlloc(CogCamera);
  self->activeCamera = CogCameraAlloc("Main Camera");
  self->lights = CogArrayAlloc(CogLight);
  self->sceneGraph = CogSceneGraphAlloc();
  
  self->name = NULL;
  self->cleanup = NULL;
  self->draw = NULL;
  self->lateUpdate = NULL;
  self->update = NULL;
  self->serialize = NULL;
  self->deserialize = NULL;
}