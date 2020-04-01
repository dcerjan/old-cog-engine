#ifndef _COG_CAMERA_H_
#define _COG_CAMERA_H_

#include "cogstring.h"
#include "cogmatrix4.h"
#include "CogVector3.h"

struct CogNode;
struct CogShader;
struct CogCamera {
  CogString    name;
  
  Bool            isOrtho;
  
  float           fov;
  float           width;
  float           height;
  float           near;
  float           far;
  
  CogMatrix4      projection;
  CogMatrix4      view;
  
  struct CogNode* anchor;
};
typedef struct CogCamera* CogCamera;
CogDefineArray(CogCamera);
CogDefineList(CogCamera);
CogDefineHash(CogCamera);

CogCamera     CogCameraAlloc        (const char* name);
void          CogCameraInspect      (CogCamera self);
void          CogCameraFree         (CogCamera self);

void          CogCameraSetFov       (CogCamera self, float fov);
void          CogCameraSetNearFar   (CogCamera self, float near, float far);
void          CogCameraSetSize      (CogCamera self, float width, float height);
void          CogCameraSetOrtho     (CogCamera self, Bool toggle);

#endif 