#ifndef _COG_QUATERNION_H_
#define _COG_QUATERNION_H_

#include "CogVector3.h"
#include "cogmatrix4.h"

typedef struct CogQuaternion {
  float x,y,z,w;
} CogQuaternion;

CogQuaternion CogQuaternionIdentity(void);
CogQuaternion CogQuaternionRotation(const CogVector3 axis, const float angle);
CogQuaternion CogQuaternionConjugate(const CogQuaternion self);
CogQuaternion CogQuaternionMultCogQuaternion(const CogQuaternion self, const CogQuaternion other);
CogQuaternion CogQuaternionNormalized(const CogQuaternion self);
CogVector3    CogQuaternionMultCogVector3(const CogQuaternion self, const CogVector3 other);
CogQuaternion CogQuaternionSlerp(const CogQuaternion src, const CogQuaternion dst, const float t);
CogMatrix4    CogQuaternionToMatrix(const CogQuaternion self);
void       CogQuaternionInspect(const CogQuaternion self);

#endif