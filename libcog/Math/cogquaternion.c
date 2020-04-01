#include "cogquaternion.h"
#include <math.h>

CogQuaternion CogQuaternionIdentity(void) {
  return (CogQuaternion) {0.0f, 0.0f, 0.0f, 1.0f};
}

CogQuaternion CogQuaternionConjugate(const CogQuaternion self) {
  return (CogQuaternion){-self.x, -self.y, -self.z, self.w};
}

CogQuaternion CogQuaternionRotation(const CogVector3 axis, const float angle) {
  float sinhalf, coshalf, a;
  a = M_PI * angle / 90.0f;
  sinhalf = sinf(a);
  coshalf = cosf(a);
  
  CogVector3 naxis = CogVector3Normalized(axis);
  
  return (CogQuaternion){coshalf * naxis.x, coshalf * naxis.y, coshalf * naxis.z, sinhalf};
}

CogQuaternion CogQuaternionMultCogQuaternion(const CogQuaternion self, const CogQuaternion other) {
  return (CogQuaternion){
      .w = self.w * other.w - self.x * other.x - self.y * other.y - self.z * other.z,
      .x = self.w * other.x + self.x * other.w + self.y * other.z - self.z * other.y,
      .y = self.w * other.y + self.y * other.w + self.z * other.x - self.x * other.z,
      .z = self.w * other.z + self.z * other.w + self.x * other.y - self.y * other.x
    };
}

CogVector3 CogQuaternionMultCogVector3(const CogQuaternion self, const CogVector3 other) {
  CogQuaternion v = {other.x, other.y, other.z, 0.0f};
  
  v = CogQuaternionMultCogQuaternion(CogQuaternionMultCogQuaternion(self, v), CogQuaternionConjugate(self));
  
  return (CogVector3){
    .x = v.x,
    .y = v.y,
    .z = v.z
  };
}

CogQuaternion CogQuaternionSlerp(const CogQuaternion src, const CogQuaternion dst, const float t) {
  float s = 1.0f - t;
  return (CogQuaternion){
    .w = src.w * s + dst.w * t,
    .x = src.x * s + dst.x * t, 
    .y = src.y * s + dst.y * t,
    .z = src.z * s + dst.z * t
  };
}

CogQuaternion CogQuaternionNormalized(const CogQuaternion self) {
  float l = 1.0f / sqrt(self.x * self.x + self.y * self.y + self.z * self.z + self.w * self.w);
  return (CogQuaternion){self.x * l, self.y * l, self.z * l, self.w * l};
}

CogMatrix4 CogQuaternionToMatrix(const CogQuaternion self) {
  CogQuaternion q = CogQuaternionNormalized(self);
  return (CogMatrix4){
    .data.a_00 = 1.0f - 2.0f*q.y*q.y - 2.0f*q.z*q.z, .data.a_01 = 2.0f*q.x*q.y - 2.0f*q.z*q.w,        .data.a_02 = 2.0f*q.x*q.z + 2.0f*q.y*q.w,         .data.a_03 = 0.0f,
    .data.a_10 = 2.0f*q.x*q.y + 2.0f*q.z*q.w,        .data.a_11 = 1.0f - 2.0f*q.x*q.x - 2.0f*q.z*q.z, .data.a_12 = 2.0f*q.y*q.z - 2.0f*q.x*q.w,         .data.a_13 = 0.0f,
    .data.a_20 = 2.0f*q.x*q.z - 2.0f*q.y*q.w,        .data.a_21 = 2.0f*q.y*q.z + 2.0f*q.x*q.w,        .data.a_22 = 1.0f - 2.0f*q.x*q.x - 2.0f*q.y*q.y,  .data.a_23 = 0.0f,
    .data.a_30 = 0.0f,                               .data.a_31 = 0.0f,                               .data.a_32 = 0.0f,                                .data.a_33 = 1.0f
  };
}

void CogQuaternionInspect(const CogQuaternion self) {
  
}