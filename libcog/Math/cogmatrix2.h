#ifndef _COG_MATRIX_2_H_
#define _COG_MATRIX_2_H_

#include "CogVector2.h"

struct CogMatrix2 {
  union {
    struct {
      float     m[4];
    } CogArray;
    
    struct {
      CogVector2   x;
      CogVector2   y;
    } vector;
    
    struct {
      float     a_00, a_01;
      float     a_10, a_11;
    } data;
  };
};
typedef struct CogMatrix2 CogMatrix2;

CogMatrix2 CogMatrix2Identity(void);
CogMatrix2 CogMatrix2Rotation(const float angle);
CogMatrix2 CogMatrix2Scale(const float s, const float t);

void CogMatrix2Set(CogMatrix2* self, float a00, float a01, float a10, float a11);
void CogMatrix2SetIdentity(CogMatrix2* self);
void CogMatrix2SetRotation(CogMatrix2* self, const float angle);
void CogMatrix2SetScale(CogMatrix2* self, const float s, const float t);

CogMatrix2 CogMatrix2MulMatrix2(const CogMatrix2 self, const CogMatrix2 other);
CogVector2 CogMatrix2MulCogVector2(const CogMatrix2 self, const CogVector2 other);

CogMatrix2 CogMatrix2Transposed(const CogMatrix2 self);
CogMatrix2 CogMatrix2Inverted(const CogMatrix2 self);
float CogMatrix2Determinant(const CogMatrix2 self);

void CogMatrix2Inspect(const CogMatrix2 self);

#endif