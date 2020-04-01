#include "cogscenegraph.h"
#include "cognode.h"
#include "cogmodel.h"
#include "coglight.h"
#include "cogmaterial.h"
#include "cogscenemanager.h"
#include "cogscene.h"
#include "cogcamera.h"

CogSceneGraph CogSceneGraphAlloc(void) {
  CogSceneGraph self = malloc(sizeof(struct CogSceneGraph));
  
  self->nodes = CogListAlloc(CogNode);
  self->root = CogNodeAlloc("root", NULL);
  
  CogListPushBack(self->nodes, self->root);
  
  return self;
}

void CogSceneGraphInspect(CogSceneGraph self) {
  
}

void CogSceneGraphFree(CogSceneGraph self) {
  while(self->nodes->size > 0) {
    CogNode del = CogListPopBack(self->nodes);
    CogNodeFree(del);
  }
  CogListFree(self->nodes);
  free(self);
}

void CogSceneGraphUpdate(CogSceneGraph self, const double t) {
  CogNodeUpdateTransforms(self->root);
  CogNodeUpdate(self->root, t);
}