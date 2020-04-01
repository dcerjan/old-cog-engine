#include "cogmatrix3.h"

#include <math.h>
#include <stdio.h>

////////////////////////////////////////////////////
//  Constructors
////////////////////////////////////////////////////
CogMatrix3 CogMatrix3Identity(void) {
  CogMatrix3 ret;
  CogMatrix3SetIdentity(&ret);
  return ret;
}
CogMatrix3 CogMatrix3Rotation3D(const CogVector3 axis, const float angle) {
  CogMatrix3 ret;
  CogMatrix3SetRotation3D(&ret, axis, angle);
  return ret;
}
CogMatrix3 CogMatrix3Rotation(const float angle) {
  CogMatrix3 ret;
  CogMatrix3SetRotation(&ret, angle);
  return ret;
}
CogMatrix3 CogMatrix3Translation(const CogVector2 translation) {
  CogMatrix3 ret;
  CogMatrix3SetTranslation(&ret, translation);
  return ret;
}
CogMatrix3 CogMatrix3Scale(const float s, const float t) {
  CogMatrix3 ret;
  CogMatrix3SetScale(&ret, s, t);
  return ret;
}
CogMatrix3 CogMatrix3Scale3D(const float s, const float t, const float u) {
  CogMatrix3 ret;
  CogMatrix3SetScale3D(&ret, s, t, u);
  return ret;
}

////////////////////////////////////////////////////
//  Setters
////////////////////////////////////////////////////
void CogMatrix3Set(CogMatrix3* self, float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22) {
  self->data.a_00 = a00; self->data.a_01 = a00; self->data.a_20 = a00;
  self->data.a_10 = a00; self->data.a_11 = a00; self->data.a_21 = a00;
  self->data.a_20 = a00; self->data.a_21 = a00; self->data.a_22 = a00;
}
void CogMatrix3SetIdentity(CogMatrix3* self) {
  self->data.a_00 = 1.0f; self->data.a_01 = 0.0f; self->data.a_20 = 0.0f;
  self->data.a_10 = 0.0f; self->data.a_11 = 1.0f; self->data.a_21 = 0.0f;
  self->data.a_20 = 0.0f; self->data.a_21 = 0.0f; self->data.a_22 = 1.0f;
}
void CogMatrix3SetRotation3D(CogMatrix3* self, const CogVector3 axis, const float angle) {
  float a = angle / 180.0f * M_PI;
  float cosfi = cosf(a);
  float sinfi = sinf(a);
  float oneminuscosfi = 1.0f - cosfi;
  
  self->data.a_00 = (cosfi + axis.x * axis.x * oneminuscosfi);
  self->data.a_01 = (axis.x * axis.y * oneminuscosfi - axis.z * sinfi);
  self->data.a_02 = (axis.x * axis.z * oneminuscosfi + axis.y * sinfi);
  
  self->data.a_10 = (axis.x * axis.y * oneminuscosfi + axis.z * sinfi);
  self->data.a_11 = (cosfi + axis.y * axis.y * oneminuscosfi);
  self->data.a_12 = (axis.y * axis.z * oneminuscosfi - axis.x * sinfi);
  
  self->data.a_20 = (axis.x * axis.z * oneminuscosfi - axis.y * sinfi);
  self->data.a_21 = (axis.y * axis.z * oneminuscosfi + axis.x * sinfi);
  self->data.a_22 = (cosfi + axis.z * axis.z * oneminuscosfi);
}
void CogMatrix3SetRotation(CogMatrix3* self, const float angle) {
  float a = angle / 180.0f * M_PI;
  float cosfi = cosf(a);
  float sinfi = sinf(a);
  
  self->data.a_00 = cosfi;
  self->data.a_01 = -sinfi;
  self->data.a_02 = 0.0f;
  
  self->data.a_10 = sinfi;
  self->data.a_11 = cosfi;
  self->data.a_12 = 0.0f;
  
  self->data.a_20 = 0.0f;
  self->data.a_21 = 0.0f;
  self->data.a_22 = 1.0f;
}
void CogMatrix3SetTranslation(CogMatrix3* self, const CogVector2 translation) {
  self->data.a_00 = 1.0f; self->data.a_01 = 0.0f; self->data.a_20 = translation.x;
  self->data.a_10 = 0.0f; self->data.a_11 = 1.0f; self->data.a_21 = translation.y;
  self->data.a_20 = 0.0f; self->data.a_21 = 0.0f; self->data.a_22 = 1.0f;
}
void CogMatrix3SetScale3D(CogMatrix3* self, const float s, const float t, const float u) {
  self->data.a_00 =    s; self->data.a_01 = 0.0f; self->data.a_20 = 0.0f;
  self->data.a_10 = 0.0f; self->data.a_11 =    t; self->data.a_21 = 0.0f;
  self->data.a_20 = 0.0f; self->data.a_21 = 0.0f; self->data.a_22 =    u;
}
void CogMatrix3SetScale(CogMatrix3* self, const float s, const float t) {
  self->data.a_00 =    s; self->data.a_01 = 0.0f; self->data.a_20 = 0.0f;
  self->data.a_10 = 0.0f; self->data.a_11 =    t; self->data.a_21 = 0.0f;
  self->data.a_20 = 0.0f; self->data.a_21 = 0.0f; self->data.a_22 = 1.0f;
}

////////////////////////////////////////////////////
//  Multiplications
////////////////////////////////////////////////////
CogMatrix3 CogMatrix3MulMatrix3(const CogMatrix3 self, const CogMatrix3 other) {
  return (CogMatrix3) {
    .vector.x.x = CogVector3Dot(self.vector.x, (CogVector3){other.vector.x.x, other.vector.y.x, other.vector.z.x}),
    .vector.y.x = CogVector3Dot(self.vector.x, (CogVector3){other.vector.x.y, other.vector.y.y, other.vector.z.y}),
    .vector.z.x = CogVector3Dot(self.vector.x, (CogVector3){other.vector.x.z, other.vector.y.z, other.vector.z.z}),
    
    .vector.x.y = CogVector3Dot(self.vector.y, (CogVector3){other.vector.x.x, other.vector.y.x, other.vector.z.x}),
    .vector.y.y = CogVector3Dot(self.vector.y, (CogVector3){other.vector.x.y, other.vector.y.y, other.vector.z.y}),
    .vector.z.y = CogVector3Dot(self.vector.y, (CogVector3){other.vector.x.z, other.vector.y.z, other.vector.z.z}),
    
    .vector.x.z = CogVector3Dot(self.vector.z, (CogVector3){other.vector.x.x, other.vector.y.x, other.vector.z.x}),
    .vector.y.z = CogVector3Dot(self.vector.z, (CogVector3){other.vector.x.y, other.vector.y.y, other.vector.z.y}),
    .vector.z.z = CogVector3Dot(self.vector.z, (CogVector3){other.vector.x.z, other.vector.y.z, other.vector.z.z})
  };
}
CogVector3 CogMatrix3MulCogVector3(const CogMatrix3 self, const CogVector3 other) {
  return (CogVector3) {
    .x = CogVector3Dot(self.vector.x, other),
    .y = CogVector3Dot(self.vector.y, other),
    .z = CogVector3Dot(self.vector.z, other),
  };
}
CogVector2 CogMatrix3MulCogVector2(const CogMatrix3 self, const CogVector2 other) {
  return (CogVector2) {
    .x = CogVector3Dot(self.vector.x, (CogVector3){other.x, other.y, 1.0f}),
    .y = CogVector3Dot(self.vector.y, (CogVector3){other.x, other.y, 1.0f})
  };
}

////////////////////////////////////////////////////
//  Other
////////////////////////////////////////////////////
CogMatrix3 CogMatrix3Transposed(const CogMatrix3 self) {
  return (CogMatrix3){
    .data.a_00 = self.data.a_00, .data.a_01 = self.data.a_10, .data.a_02 = self.data.a_20,
    .data.a_10 = self.data.a_01, .data.a_11 = self.data.a_11, .data.a_12 = self.data.a_21,
    .data.a_20 = self.data.a_02, .data.a_21 = self.data.a_12, .data.a_22 = self.data.a_22
  };
}
CogMatrix3 CogMatrix3Inverted(const CogMatrix3 self) {
  
  CogMatrix3 ret;
  
  float d00 = self.vector.y.y * self.vector.z.z - self.vector.y.z * self.vector.z.y;
  float d01 = self.vector.y.x * self.vector.z.z - self.vector.y.z * self.vector.z.x;
  float d02 = self.vector.y.x * self.vector.z.y - self.vector.y.y * self.vector.z.x;
  
  float d10 = self.vector.x.y * self.vector.z.z - self.vector.x.z * self.vector.z.y;
  float d11 = self.vector.x.x * self.vector.z.z - self.vector.x.z * self.vector.z.x;
  float d12 = self.vector.x.x * self.vector.z.y - self.vector.x.y * self.vector.z.x;
  
  float d20 = self.vector.x.y * self.vector.y.z - self.vector.x.z * self.vector.y.y;
  float d21 = self.vector.x.x * self.vector.y.z - self.vector.x.z * self.vector.z.x;
  float d22 = self.vector.x.x * self.vector.y.y - self.vector.x.y * self.vector.y.x;
  
  float d = 1.0f / (self.vector.x.x * d00 + self.vector.x.y * d01 + self.vector.x.z * d02);
  
  ret.data.a_00 = +d00 * d;
  ret.data.a_01 = -d10 * d;
  ret.data.a_02 = +d20 * d;
  
  ret.data.a_10 = +d01 * d;
  ret.data.a_11 = +d11 * d;
  ret.data.a_12 = +d21 * d;
  
  ret.data.a_20 = +d02 * d;
  ret.data.a_21 = +d12 * d;
  ret.data.a_22 = +d22 * d;
  
  return ret;
}
float CogMatrix3Determinant(const CogMatrix3 self) {
  return self.vector.x.x * (self.vector.y.y * self.vector.z.z - self.vector.y.z * self.vector.z.y) + 
         self.vector.x.y * (self.vector.y.z * self.vector.z.x - self.vector.y.x * self.vector.z.z) +
         self.vector.x.z * (self.vector.y.x * self.vector.z.y - self.vector.y.y * self.vector.z.x);
}

void CogMatrix3Inspect(const CogMatrix3 self) {
  printf("<Matrix3 (\n");
  printf("  "); CogVector3Inspect(self.vector.x);
  printf("  "); CogVector3Inspect(self.vector.y);
  printf("  "); CogVector3Inspect(self.vector.z);
  printf(")\n>\n");
}
