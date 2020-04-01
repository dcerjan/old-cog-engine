#include "cogskeleton.h"
#include "cogbundle.h"

CogSkeleton CogSkeletonAlloc(const char* name, struct CogBundle* bundle) {
  CogSkeleton self = malloc(sizeof(struct CogSkeleton));
  
  self->name = CogStringAlloc(name);
  self->animations = CogHashAlloc(CogSkeletonAnimation);
  self->bones = CogArrayAlloc(CogBone);
  self->playingAnimations = CogArrayAlloc(CogSkeletonAnimation);
  
  if(bundle) {
    CogBundleAddSkeleton(bundle, self);
  }
  
  return self;
}

void CogSkeletonInspect(const CogSkeleton self) {
  
}

void CogSkeletonFree(CogSkeleton self) {
  
}