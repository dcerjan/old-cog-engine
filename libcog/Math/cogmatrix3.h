#ifndef _COG_MATRIX_3_H_
#define _COG_MATRIX_3_H_

#include "CogVector2.h"
#include "CogVector3.h"

struct CogMatrix3 {
  union {
    struct {
      float     m[9];
    } CogArray;
    
    struct {
      CogVector3   x;
      CogVector3   y;
      CogVector3   z;
    } vector;
    
    struct {
      float     a_00, a_01, a_02;
      float     a_10, a_11, a_12;
      float     a_20, a_21, a_22;
    } data;
  };
};
typedef struct CogMatrix3 CogMatrix3;

CogMatrix3 CogMatrix3Identity(void);
CogMatrix3 CogMatrix3Rotation3D(const CogVector3 axis, const float angle);
CogMatrix3 CogMatrix3Rotation(const float angle);
CogMatrix3 CogMatrix3Translation(const CogVector2 translation);
CogMatrix3 CogMatrix3Scale(const float s, const float t);
CogMatrix3 CogMatrix3Scale3D(const float s, const float t, const float u);

void CogMatrix3Set(CogMatrix3* self, float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);
void CogMatrix3SetIdentity(CogMatrix3* self);
void CogMatrix3SetRotation(CogMatrix3* self,const float angle);
void CogMatrix3SetRotation3D(CogMatrix3* self, const CogVector3 axis, const float angle);
void CogMatrix3SetTranslation(CogMatrix3* self, const CogVector2 translation);
void CogMatrix3SetScale(CogMatrix3* self, const float s, const float t);
void CogMatrix3SetScale3D(CogMatrix3* self, const float s, const float t, const float u);

CogMatrix3 CogMatrix3MulMatrix3(const CogMatrix3 self, const CogMatrix3 other);
CogVector3 CogMatrix3MulCogVector3(const CogMatrix3 self, const CogVector3 other);
CogVector2 CogMatrix3MulCogVector2(const CogMatrix3 self, const CogVector2 other);

CogMatrix3 CogMatrix3Transposed(const CogMatrix3 self);
CogMatrix3 CogMatrix3Inverted(const CogMatrix3 self);
float CogMatrix3Determinant(const CogMatrix3 self);

void CogMatrix3Inspect(const CogMatrix3 self);

#endif