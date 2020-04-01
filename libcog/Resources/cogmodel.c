#include "cogmodel.h"
#include "cogbundle.h"

CogModel CogModelAlloc(const char* name, CogMesh mesh, CogMaterial material, CogSkeleton skeleton, struct CogBundle* bundle) {
  CogModel self = malloc(sizeof(struct CogModel));
  
  self->name = CogStringAlloc(name);
  self->mesh = mesh;
  self->material = material;
  self->skeleton = skeleton;
  
  if(bundle) {
    CogBundleAddModel(bundle, self);
  }
  
  return self;
}

void CogModelInspect(const CogModel self) {
  
}

void CogModelFree(CogModel self) {
  CogStringFree(self->name);
  free(self);
}

void CogModelUpdate(CogModel self, double t) {
  
}

void CogModelPlayAnimation(CogModel self, const char* animation, const float advanceSpeed,const Bool loop) {
  
}