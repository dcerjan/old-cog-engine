#include "cogmatrix4.h"
#include "cogmath.h"

#include <math.h>

#define DET3X3( a00, a10, a20, \
                a01, a11, a21, \
                a02, a12, a22 ) \
                ( (a00) * ((a11)*(a22) - (a21)*(a12)) + \
                  (a10) * ((a21)*(a02) - (a01)*(a22)) + \
                  (a20) * ((a01)*(a12) - (a11)*(a02)))

////////////////////////////////////////////////////
//  Constructors
////////////////////////////////////////////////////
CogMatrix4 CogMatrix4Rotation(const CogVector3 axis, const float angle) {
  CogMatrix4 ret;
  CogMatrix4SetRotation(&ret, axis, angle);
  return ret;
}
CogMatrix4 CogMatrix4Translation(const CogVector3 position) {
  CogMatrix4 ret;
  CogMatrix4SetTranslation(&ret, position);
  return ret;
}
CogMatrix4 CogMatrix4Scale(const float s, const float t, const float u) {
  CogMatrix4 ret;
  CogMatrix4SetScale(&ret, s, t, u);
  return ret;
}
CogMatrix4 CogMatrix4Identity(void) {
  CogMatrix4 ret;
  CogMatrix4SetIdentity(&ret);
  return ret;
}
CogMatrix4 CogMatrix4LookAt(const CogVector3 eye, const CogVector3 target, const CogVector3 up) {
  CogMatrix4 ret;
  CogMatrix4SetLookAt(&ret, eye, target, up);
  return ret;
}
CogMatrix4 CogMatrix4Ortho(const float left, const float right, const float bottom, const float top, const float near, const float far) {
  CogMatrix4 ret;
  CogMatrix4SetOrtho(&ret, left, right, bottom, top, near, far);
  return ret;
}
CogMatrix4 CogMatrix4Frustum(const float left, const float right, const float bottom, const float top, const float near, const float far) {
  CogMatrix4 ret;
  CogMatrix4SetFrustum(&ret, left, right, bottom, top, near, far);
  return ret;
}
CogMatrix4 CogMatrix4Perspective(const float fovy, const float aspectRatio, const float near, const float far) {
  CogMatrix4 ret;
  CogMatrix4SetPerspective(&ret, fovy, aspectRatio, near, far);
  return ret;
}

////////////////////////////////////////////////////
//  Setters
////////////////////////////////////////////////////
void CogMatrix4Set(CogMatrix4* self, const float a00, const float a01, const float a02, const float a03, 
                               const float a10, const float a11, const float a12, const float a13, 
                               const float a20, const float a21, const float a22, const float a23, 
                               const float a30, const float a31, const float a32, const float a33) {
  self->data.a_00 = a00;
  self->data.a_01 = a01;
  self->data.a_02 = a02;
  self->data.a_03 = a03;
  
  self->data.a_10 = a10;
  self->data.a_11 = a11;
  self->data.a_12 = a12;
  self->data.a_13 = a13;
  
  self->data.a_20 = a20;
  self->data.a_21 = a21;
  self->data.a_22 = a22;
  self->data.a_23 = a23;
  
  self->data.a_30 = a30;
  self->data.a_31 = a31;
  self->data.a_32 = a32;
  self->data.a_33 = a33;
}
void CogMatrix4SetRotation(CogMatrix4* self, const CogVector3 axis, const float angle) {
  float a = angle / 180.0f * M_PI;
  float cosfi = cosf(a);
  float sinfi = sinf(a);
  float oneminuscosfi = 1.0f - cosfi;
  
  self->vector.x.x = (cosfi + axis.x * axis.x * oneminuscosfi);
  self->vector.x.y = (axis.x * axis.y * oneminuscosfi - axis.z * sinfi);
  self->vector.x.z = (axis.x * axis.z * oneminuscosfi + axis.y * sinfi);
  self->vector.x.w = 0.0f;
  
  self->vector.y.x = (axis.x * axis.y * oneminuscosfi + axis.z * sinfi);
  self->vector.y.y = (cosfi + axis.y * axis.y * oneminuscosfi);
  self->vector.y.z = (axis.y * axis.z * oneminuscosfi - axis.x * sinfi);
  self->vector.y.w = 0.0f;
  
  self->vector.z.x = (axis.x * axis.z * oneminuscosfi - axis.y * sinfi);
  self->vector.z.y = (axis.y * axis.z * oneminuscosfi + axis.x * sinfi);
  self->vector.z.z = (cosfi + axis.z * axis.z * oneminuscosfi);
  self->vector.z.w = 0.0f;
  
  self->vector.w.x = 0.0f;
  self->vector.w.y = 0.0f;
  self->vector.w.z = 0.0f;
  self->vector.w.w = 1.0f;
}
void CogMatrix4SetScale(CogMatrix4* self, const float s, const float t, const float u) {
  self->vector.x.x = s;
  self->vector.x.y = 0.0f;
  self->vector.x.z = 0.0f;
  self->vector.x.w = 0.0f;
  
  self->vector.y.x = 0.0f;
  self->vector.y.y = t;
  self->vector.y.z = 0.0f;
  self->vector.y.w = 0.0f;
  
  self->vector.z.x = 0.0f;
  self->vector.z.y = 0.0f;
  self->vector.z.z = u;
  self->vector.z.w = 0.0f;
  
  self->vector.w.x = 0.0f;
  self->vector.w.y = 0.0f;
  self->vector.w.z = 0.0f;
  self->vector.w.w = 1.0f;
}
void CogMatrix4SetTranslation(CogMatrix4* self, const CogVector3 position) {
  self->vector.x.x = 1.0f;
  self->vector.x.y = 0.0f;
  self->vector.x.z = 0.0f;
  self->vector.x.w = 0.0f;
  
  self->vector.y.x = 0.0f;
  self->vector.y.y = 1.0f;
  self->vector.y.z = 0.0f;
  self->vector.y.w = 0.0f;
  
  self->vector.z.x = 0.0f;
  self->vector.z.y = 0.0f;
  self->vector.z.z = 1.0f;
  self->vector.z.w = 0.0f;
  
  self->vector.w.x = position.x;
  self->vector.w.y = position.y;
  self->vector.w.z = position.z;
  self->vector.w.w = 1.0f;
}
void CogMatrix4SetIdentity(CogMatrix4* self) {
  self->vector.x.x = 1.0f;
  self->vector.x.y = 0.0f;
  self->vector.x.z = 0.0f;
  self->vector.x.w = 0.0f;
  
  self->vector.y.x = 0.0f;
  self->vector.y.y = 1.0f;
  self->vector.y.z = 0.0f;
  self->vector.y.w = 0.0f;
  
  self->vector.z.x = 0.0f;
  self->vector.z.y = 0.0f;
  self->vector.z.z = 1.0f;
  self->vector.z.w = 0.0f;
  
  self->vector.w.x = 0.0f;
  self->vector.w.y = 0.0f;
  self->vector.w.z = 0.0f;
  self->vector.w.w = 1.0f;
}
void CogMatrix4SetLookAt(CogMatrix4* self, const CogVector3 eye, const CogVector3 target, const CogVector3 up) {
  CogVector3 tmp, fwd, right, up2;
  
  tmp = CogVector3Sub(target, eye);
  fwd = CogVector3Normalized(tmp);
  
  tmp = CogVector3Cross(fwd, up);
  right = CogVector3Normalized(tmp);
  
  tmp = CogVector3Cross(right, fwd);
  up2 = CogVector3Normalized(tmp);
  
  self->vector.x.x =  right.x;
  self->vector.x.y =  right.y;
  self->vector.x.z =  right.z;
  self->vector.x.w = 0.0f;
  
  self->vector.y.x =  up2.x;
  self->vector.y.y =  up2.y;
  self->vector.y.z =  up2.z;
  self->vector.y.w = 0.0f;
  
  self->vector.z.x = -fwd.x;
  self->vector.z.y = -fwd.y;
  self->vector.z.z = -fwd.z;
  self->vector.z.w = 0.0f;
  
  self->vector.w.x = CogVector3Dot(right, eye);
  self->vector.w.y = CogVector3Dot(up2, eye);
  self->vector.w.z = -CogVector3Dot(fwd, eye);
  self->vector.w.w = 1.0f;
}
void CogMatrix4SetOrtho(CogMatrix4*  self, const float left, const float right, const float bottom, const float top, const float near, const float far) {
  self->vector.x.x = 2.0f / (right - left);
  self->vector.x.y = 0.0f;
  self->vector.x.z = 0.0f;
  self->vector.x.w = 0.0f;
  
  self->vector.y.x = 0.0f;
  self->vector.y.y = 2.0f / (top - bottom);
  self->vector.y.z = 0.0f;
  self->vector.y.w = 0.0f;
  
  self->vector.z.x = 0.0f;
  self->vector.z.y = 0.0f;
  self->vector.z.z = -2.0f / (far - near);
  self->vector.z.w = 0.0f;
  
  self->vector.w.x = - (right + left) / (right - left);;
  self->vector.w.y = - (top + bottom) / (top - bottom);
  self->vector.w.z = - (far + near) / (far - near);
  self->vector.w.w = 1.0f;
}
void CogMatrix4SetFrustum(CogMatrix4* self, const float left, const float right, const float bottom, const float top, const float near, const float far) {
  self->vector.x.x = 2.0f * near / (right - left);
  self->vector.x.y = 0.0f;
  self->vector.x.z = 0.0f;
  self->vector.x.w = 0.0f;
  
  self->vector.y.x = 0.0f;
  self->vector.y.y = 2.0f * near / (top - bottom);
  self->vector.y.z = 0.0f;
  self->vector.y.w = 0.0f;
  
  self->vector.z.x = (right + left) / (right - left);
  self->vector.z.y = (top + bottom) / (top - bottom);
  self->vector.z.z =-(far + near) / (far - near);
  self->vector.z.w =-1.0f;
  
  self->vector.w.x = 0.0f;
  self->vector.w.y = 0.0f;
  self->vector.w.z =-2.0f * far * near / (far - near);
  self->vector.w.w = 0.0f;
}
void CogMatrix4SetPerspective(CogMatrix4* self, const float fovy, const float ratio, const float near, const float far) {
  float scale = tan(fovy / 360.0 * M_PI) * near;
  float r = ratio * scale, l = -r;
  float t = scale, b = -t;
  
  CogMatrix4SetFrustum(self, l, r, b, t, near, far);
}
////////////////////////////////////////////////////
//  Multiplications
////////////////////////////////////////////////////
CogVector3 CogMatrix4MulCogVector3(const CogMatrix4 self, const CogVector3 other) {
  CogVector3 ret = {
    .x = (self.vector.x.x * other.x + self.vector.y.x * other.y + self.vector.z.x * other.z + self.vector.w.x),
    .y = (self.vector.x.y * other.x + self.vector.y.y * other.y + self.vector.z.y * other.z + self.vector.w.y),
    .z = (self.vector.x.z * other.x + self.vector.y.z * other.y + self.vector.z.z * other.z + self.vector.w.z)
  };
  return ret;
}
CogVector4 CogMatrix4MulVector4(const CogMatrix4 self, const CogVector4 other) {
  CogVector4 ret = {
    .x = (self.vector.x.x * other.x + self.vector.y.x * other.y + self.vector.z.x * other.z + self.vector.w.x * other.w),
    .y = (self.vector.x.y * other.x + self.vector.y.y * other.y + self.vector.z.y * other.z + self.vector.w.y * other.w),
    .z = (self.vector.x.z * other.x + self.vector.y.z * other.y + self.vector.z.z * other.z + self.vector.w.z * other.w),
    .w = (self.vector.x.w * other.x + self.vector.y.w * other.y + self.vector.z.w * other.z + self.vector.w.w * other.w)
  };
  return ret;
}
CogMatrix4 CogMatrix4MulMatrix4(const CogMatrix4 self, const CogMatrix4 other) {
  CogMatrix4 ret;
  
  ret.vector.x.x = CogVector4Dot(other.vector.x, (CogVector4){self.vector.x.x, self.vector.y.x, self.vector.z.x, self.vector.w.x});
  ret.vector.x.y = CogVector4Dot(other.vector.x, (CogVector4){self.vector.x.y, self.vector.y.y, self.vector.z.y, self.vector.w.y});
  ret.vector.x.z = CogVector4Dot(other.vector.x, (CogVector4){self.vector.x.z, self.vector.y.z, self.vector.z.z, self.vector.w.z});
  ret.vector.x.w = CogVector4Dot(other.vector.x, (CogVector4){self.vector.x.w, self.vector.y.w, self.vector.z.w, self.vector.w.w});
  
  ret.vector.y.x = CogVector4Dot(other.vector.y, (CogVector4){self.vector.x.x, self.vector.y.x, self.vector.z.x, self.vector.w.x});
  ret.vector.y.y = CogVector4Dot(other.vector.y, (CogVector4){self.vector.x.y, self.vector.y.y, self.vector.z.y, self.vector.w.y});
  ret.vector.y.z = CogVector4Dot(other.vector.y, (CogVector4){self.vector.x.z, self.vector.y.z, self.vector.z.z, self.vector.w.z});
  ret.vector.y.w = CogVector4Dot(other.vector.y, (CogVector4){self.vector.x.w, self.vector.y.w, self.vector.z.w, self.vector.w.w});
  
  ret.vector.z.x = CogVector4Dot(other.vector.z, (CogVector4){self.vector.x.x, self.vector.y.x, self.vector.z.x, self.vector.w.x});
  ret.vector.z.y = CogVector4Dot(other.vector.z, (CogVector4){self.vector.x.y, self.vector.y.y, self.vector.z.y, self.vector.w.y});
  ret.vector.z.z = CogVector4Dot(other.vector.z, (CogVector4){self.vector.x.z, self.vector.y.z, self.vector.z.z, self.vector.w.z});
  ret.vector.z.w = CogVector4Dot(other.vector.z, (CogVector4){self.vector.x.w, self.vector.y.w, self.vector.z.w, self.vector.w.w});
  
  ret.vector.w.x = CogVector4Dot(other.vector.w, (CogVector4){self.vector.x.x, self.vector.y.x, self.vector.z.x, self.vector.w.x});
  ret.vector.w.y = CogVector4Dot(other.vector.w, (CogVector4){self.vector.x.y, self.vector.y.y, self.vector.z.y, self.vector.w.y});
  ret.vector.w.z = CogVector4Dot(other.vector.w, (CogVector4){self.vector.x.z, self.vector.y.z, self.vector.z.z, self.vector.w.z});
  ret.vector.w.w = CogVector4Dot(other.vector.w, (CogVector4){self.vector.x.w, self.vector.y.w, self.vector.z.w, self.vector.w.w});
  
  return ret;
}
////////////////////////////////////////////////////
//  Other
////////////////////////////////////////////////////
CogMatrix4 CogMatrix4Transposed(const CogMatrix4 self) {
  return (CogMatrix4) {
    .data = {
      self.data.a_00, self.data.a_10, self.data.a_20, self.data.a_30,
      self.data.a_01, self.data.a_11, self.data.a_21, self.data.a_31,
      self.data.a_02, self.data.a_12, self.data.a_22, self.data.a_32,
      self.data.a_03, self.data.a_13, self.data.a_23, self.data.a_33
    }
  };
}

CogMatrix4 CogMatrix4Inverted(const CogMatrix4 self) {
  CogMatrix4 ret;
  
  float d00 = DET3X3(self.data.a_11, self.data.a_12, self.data.a_13, 
                     self.data.a_21, self.data.a_22, self.data.a_23, 
                     self.data.a_31, self.data.a_32, self.data.a_33);
                     
  float d01 = DET3X3(self.data.a_10, self.data.a_12, self.data.a_13, 
                     self.data.a_20, self.data.a_22, self.data.a_23, 
                     self.data.a_30, self.data.a_32, self.data.a_33);
                     
  float d02 = DET3X3(self.data.a_10, self.data.a_11, self.data.a_13, 
                     self.data.a_20, self.data.a_21, self.data.a_23, 
                     self.data.a_30, self.data.a_31, self.data.a_33);
  
  float d03 = DET3X3(self.data.a_10, self.data.a_11, self.data.a_12, 
                     self.data.a_20, self.data.a_21, self.data.a_22, 
                     self.data.a_30, self.data.a_31, self.data.a_32);
                     
                     
  float d10 = DET3X3(self.data.a_01, self.data.a_02, self.data.a_03, 
                     self.data.a_21, self.data.a_22, self.data.a_23, 
                     self.data.a_31, self.data.a_32, self.data.a_33);
                     
  float d11 = DET3X3(self.data.a_00, self.data.a_02, self.data.a_03, 
                     self.data.a_20, self.data.a_22, self.data.a_23, 
                     self.data.a_30, self.data.a_32, self.data.a_33);
                     
  float d12 = DET3X3(self.data.a_00, self.data.a_01, self.data.a_03, 
                     self.data.a_20, self.data.a_21, self.data.a_23, 
                     self.data.a_30, self.data.a_31, self.data.a_33);
                     
  float d13 = DET3X3(self.data.a_00, self.data.a_01, self.data.a_02, 
                     self.data.a_20, self.data.a_21, self.data.a_22, 
                     self.data.a_30, self.data.a_31, self.data.a_32);   
                     
                     
  float d20 = DET3X3(self.data.a_01, self.data.a_02, self.data.a_03, 
                     self.data.a_11, self.data.a_12, self.data.a_13, 
                     self.data.a_31, self.data.a_32, self.data.a_33);
                     
  float d21 = DET3X3(self.data.a_00, self.data.a_02, self.data.a_03, 
                     self.data.a_10, self.data.a_12, self.data.a_13, 
                     self.data.a_30, self.data.a_32, self.data.a_33);
                     
  float d22 = DET3X3(self.data.a_00, self.data.a_01, self.data.a_03, 
                     self.data.a_10, self.data.a_11, self.data.a_13, 
                     self.data.a_30, self.data.a_31, self.data.a_33);
                     
  float d23 = DET3X3(self.data.a_00, self.data.a_01, self.data.a_02, 
                     self.data.a_10, self.data.a_11, self.data.a_12, 
                     self.data.a_30, self.data.a_31, self.data.a_32);
                     
                     
  float d30 = DET3X3(self.data.a_01, self.data.a_02, self.data.a_03, 
                     self.data.a_11, self.data.a_12, self.data.a_13, 
                     self.data.a_21, self.data.a_22, self.data.a_23);
                     
  float d31 = DET3X3(self.data.a_00, self.data.a_02, self.data.a_03, 
                     self.data.a_10, self.data.a_12, self.data.a_13, 
                     self.data.a_20, self.data.a_22, self.data.a_23);  
                   
  float d32 = DET3X3(self.data.a_00, self.data.a_01, self.data.a_03, 
                     self.data.a_10, self.data.a_11, self.data.a_13, 
                     self.data.a_20, self.data.a_21, self.data.a_23);
                     
  float d33 = DET3X3(self.data.a_00, self.data.a_01, self.data.a_02, 
                     self.data.a_10, self.data.a_11, self.data.a_12, 
                     self.data.a_20, self.data.a_21, self.data.a_22);
  
  float d = 1.0f / (self.data.a_00 * d00 - self.data.a_01 * d01 + self.data.a_02 * d02 - self.data.a_03 * d03);
  
  ret.data.a_00 = +d00 * d;
  ret.data.a_01 = -d10 * d;
  ret.data.a_02 = +d20 * d;
  ret.data.a_03 = -d30 * d;
  
  ret.data.a_10 = -d01 * d;
  ret.data.a_11 = +d11 * d;
  ret.data.a_12 = -d21 * d;
  ret.data.a_13 = +d31 * d;
  
  ret.data.a_20 = +d02 * d;
  ret.data.a_21 = -d12 * d;
  ret.data.a_22 = +d22 * d;
  ret.data.a_23 = -d32 * d;
  
  ret.data.a_30 = -d03 * d;
  ret.data.a_31 = +d13 * d;
  ret.data.a_32 = -d23 * d;
  ret.data.a_33 = +d33 * d;
  
  return ret;
}
float CogMatrix4Determinant(const CogMatrix4 self) {
  float d00 = DET3X3(self.data.a_11, self.data.a_12, self.data.a_13, 
                     self.data.a_21, self.data.a_22, self.data.a_23, 
                     self.data.a_31, self.data.a_32, self.data.a_33);
                     
  float d01 = DET3X3(self.data.a_10, self.data.a_12, self.data.a_13, 
                     self.data.a_20, self.data.a_22, self.data.a_23, 
                     self.data.a_30, self.data.a_32, self.data.a_33);
                     
  float d02 = DET3X3(self.data.a_10, self.data.a_11, self.data.a_13, 
                     self.data.a_20, self.data.a_21, self.data.a_23, 
                     self.data.a_30, self.data.a_31, self.data.a_33);
  
  float d03 = DET3X3(self.data.a_10, self.data.a_11, self.data.a_12, 
                     self.data.a_20, self.data.a_21, self.data.a_22, 
                     self.data.a_30, self.data.a_31, self.data.a_32);
  
  return (self.data.a_00 * d00 - self.data.a_01 * d01 + self.data.a_02 * d02 - self.data.a_03 * d03);
}

void CogMatrix4Inspect(const CogMatrix4 self) {
  printf("<CogMatrix4 (\n");
  printf("  "); CogVector4Inspect(self.vector.x);
  printf("  "); CogVector4Inspect(self.vector.y);
  printf("  "); CogVector4Inspect(self.vector.z);
  printf("  "); CogVector4Inspect(self.vector.w);
  printf(")\n>\n");
}
