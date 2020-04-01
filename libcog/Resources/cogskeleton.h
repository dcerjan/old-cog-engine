#ifndef _COG_SKELETON_H_
#define _COG_SKELETON_H_

#include "CogArray.h"
#include "CogHash.h"

#include "cogstring.h"
#include "cogquaternion.h"


/* Bone */
struct CogBone {
  CogString      name;
  CogQuaternion  rotation;
};
typedef struct CogBone* CogBone;
CogDefineArray(CogBone);




/* Keyframe */
struct CogSkeletonKeyframe {
  float             timestamp;
  CogArray(CogBone) bones;
};
typedef struct CogSkeletonKeyframe* CogSkeletonKeyframe;
CogDefineArray(CogSkeletonKeyframe);




/* Animation */
struct CogSkeletonAnimation {
  CogString                     name;
  float                         duration;
  CogArray(CogSkeletonKeyframe) keyframes;
};
typedef struct CogSkeletonAnimation* CogSkeletonAnimation;
CogDefineHash(CogSkeletonAnimation);
CogDefineArray(CogSkeletonAnimation);



/* Skeleton */
struct CogBundle;
struct CogSkeleton {
  CogString                      name;
  CogArray(CogBone)              bones;
  CogHash(CogSkeletonAnimation)  animations;
  CogArray(CogSkeletonAnimation) playingAnimations;
};
typedef struct CogSkeleton* CogSkeleton;
CogDefineHash(CogSkeleton);

CogSkeleton CogSkeletonAlloc(const char* name, struct CogBundle* bundle);
void CogSkeletonInspect(const CogSkeleton self);
void CogSkeletonFree(CogSkeleton self);

#endif