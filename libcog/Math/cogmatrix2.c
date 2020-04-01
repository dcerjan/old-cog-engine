#include "cogmatrix2.h"

#include <math.h>
#include <stdio.h>

////////////////////////////////////////////////////
//  Constructors
////////////////////////////////////////////////////
CogMatrix2 CogMatrix2Identity(void) {
  CogMatrix2 ret;
  CogMatrix2SetIdentity(&ret);
  return ret;
}
CogMatrix2 CogMatrix2Rotation(const float angle) {
  CogMatrix2 ret;
  CogMatrix2SetRotation(&ret, angle);
  return ret;
}
CogMatrix2 CogMatrix2Scale(const float s, const float t) {
  CogMatrix2 ret;
  CogMatrix2SetScale(&ret, s, t);
  return ret;
}

////////////////////////////////////////////////////
//  Setters
////////////////////////////////////////////////////
void CogMatrix2Set(CogMatrix2* self, float a00, float a01, float a10, float a11) {
  self->data.a_00 = a00; self->data.a_00 = a01;
  self->data.a_10 = a00; self->data.a_00 = a11;
}
void CogMatrix2SetIdentity(CogMatrix2* self) {
  self->data.a_00 = 1.0f; self->data.a_00 = 0.0f;
  self->data.a_10 = 0.0f; self->data.a_00 = 1.0f;
}
void CogMatrix2SetRotation(CogMatrix2* self, const float angle) {
  float a = angle / 180.0f * M_PI;
  float cosfi = cosf(a);
  float sinfi = sinf(a);
  self->data.a_00 = cosfi; self->data.a_01 = -sinfi;
  self->data.a_00 = sinfi; self->data.a_01 =  cosfi;
}
void CogMatrix2SetScale(CogMatrix2* self, const float s, const float t) {
  self->data.a_00 =    s; self->data.a_00 = 0.0f;
  self->data.a_10 = 0.0f; self->data.a_00 =    t;
}

////////////////////////////////////////////////////
//  Multiplications
////////////////////////////////////////////////////
CogMatrix2 CogMatrix2MulMatrix2(const CogMatrix2 self, const CogMatrix2 other) {
  return (CogMatrix2) {
    .vector.x.x = CogVector2Dot(self.vector.x, (CogVector2){other.vector.x.x, other.vector.y.x}),
    .vector.y.x = CogVector2Dot(self.vector.x, (CogVector2){other.vector.x.y, other.vector.y.y}),
    
    .vector.x.y = CogVector2Dot(self.vector.y, (CogVector2){other.vector.x.x, other.vector.y.x}),
    .vector.y.y = CogVector2Dot(self.vector.y, (CogVector2){other.vector.x.y, other.vector.y.y}),
  };
}
CogVector2 CogMatrix2MulCogVector2(const CogMatrix2 self, const CogVector2 other) {
  return (CogVector2){
    .x = CogVector2Dot(self.vector.x, other), 
    .y = CogVector2Dot(self.vector.y, other)
  };
}

////////////////////////////////////////////////////
//  Other
////////////////////////////////////////////////////
CogMatrix2 CogMatrix2Transposed(const CogMatrix2 self) {
  return (CogMatrix2) {
    .vector.x.x = self.vector.x.x,
    .vector.x.y = self.vector.y.x,
    
    .vector.y.x = self.vector.x.y,
    .vector.y.y = self.vector.y.y,
  };
}
CogMatrix2 CogMatrix2Inverted(const CogMatrix2 self) {
  CogMatrix2 ret;
  
  float d = 1.0f / (self.vector.x.x * self.vector.y.y - self.vector.x.y * self.vector.y.x);
  
  ret.data.a_00 = +self.vector.x.x * d;
  ret.data.a_01 = -self.vector.y.x * d;
  
  ret.data.a_10 = -self.vector.x.y * d;
  ret.data.a_11 = +self.vector.y.y * d;
  
  return ret;
}
float CogMatrix2Determinant(const CogMatrix2 self) {
  return self.vector.x.x * self.vector.y.y - self.vector.x.y * self.vector.y.x;
}

void CogMatrix2Inspect(const CogMatrix2 self) {
  printf("<CogMatrix2 (\n");
  printf("  "); CogVector2Inspect(self.vector.x);
  printf("  "); CogVector2Inspect(self.vector.y);
  printf(")\n>\n");
}
