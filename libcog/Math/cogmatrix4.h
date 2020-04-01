#ifndef _COG_MATRIX_4_H_
#define _COG_MATRIX_4_H_

#include "cogvector4.h"
#include "CogVector3.h"

struct CogMatrix4 {
  union {
    struct {
      float     m[16];
    } CogArray;
    
    struct {
      CogVector4   x;
      CogVector4   y;
      CogVector4   z;
      CogVector4   w;
    } vector;
    
    struct {
      float     a_00, a_01, a_02, a_03;
      float     a_10, a_11, a_12, a_13;
      float     a_20, a_21, a_22, a_23;
      float     a_30, a_31, a_32, a_33;
    } data;
  };
};
typedef struct CogMatrix4 CogMatrix4;

CogMatrix4 CogMatrix4Identity(void);
CogMatrix4 CogMatrix4Rotation(const CogVector3 axis, const float angle);
CogMatrix4 CogMatrix4Translation(const CogVector3 translation);
CogMatrix4 CogMatrix4Scale(const float s, const float t, const float u);
CogMatrix4 CogMatrix4Ortho(const float left, const float right, const float top, const float bottom, const float far, const float near);
CogMatrix4 CogMatrix4Frustum(const float left, const float right, const float top, const float bottom, const float far, const float near);
CogMatrix4 CogMatrix4Perspective(const float fovy, const float aspectRatio, const float near, const float far);
CogMatrix4 CogMatrix4LookAt(const CogVector3 from, const CogVector3 to, const CogVector3 up);

void CogMatrix4Set(CogMatrix4* self, const float a00, const float a01, const float a02, const float a03, 
                               const float a10, const float a11, const float a12, const float a13, 
                               const float a20, const float a21, const float a22, const float a23, 
                               const float a30, const float a31, const float a32, const float a33);
void CogMatrix4SetIdentity(CogMatrix4* self);
void CogMatrix4SetRotation(CogMatrix4* self, const CogVector3 axis, const float angle);
void CogMatrix4SetTranslation(CogMatrix4* self, const CogVector3 translation);
void CogMatrix4SetScale(CogMatrix4* self, const float s, const float t, const float u);
void CogMatrix4SetOrtho(CogMatrix4* self, const float left, const float right, const float top, const float bottom, const float far, const float near);
void CogMatrix4SetFrustum(CogMatrix4* self, const float left, const float right, const float top, const float bottom, const float far, const float near);
void CogMatrix4SetPerspective(CogMatrix4* self, const float fovy, const float aspectRatio, const float near, const float far);
void CogMatrix4SetLookAt(CogMatrix4* self, const CogVector3 from, const CogVector3 to, const CogVector3 up);

CogMatrix4 CogMatrix4MulMatrix4(const CogMatrix4 self, const CogMatrix4 other);
CogVector4 CogMatrix4MulVector4(const CogMatrix4 self, const CogVector4 other);
CogVector3 CogMatrix4MulCogVector3(const CogMatrix4 self, const CogVector3 other);

CogMatrix4 CogMatrix4Transposed(const CogMatrix4 self);
CogMatrix4 CogMatrix4Inverted(const CogMatrix4 self);
float CogMatrix4Determinant(const CogMatrix4 self);

void CogMatrix4Inspect(const CogMatrix4 self);

#endif
