#include "cogcamera.h"
#include "cogshader.h"

CogCamera CogCameraAlloc(const char* name) {
  CogCamera self = malloc(sizeof(struct CogCamera));
  self->anchor = NULL;
  self->fov = 75.0f;
  self->height = 768.0f;
  self->width = 1024.0f;
  self->isOrtho = False;
  self->name = CogStringAlloc(name);
  self->near = 0.1f;
  self->far = 100.0f;
  self->projection = CogMatrix4Perspective(self->fov, self->width / (self->height > 0.0f ? self->height : 1.0f), self->near, self->far);
  self->view = CogMatrix4Identity();
  return self;
}

void CogCameraInspect(CogCamera self) {
  
}

void CogCameraFree(CogCamera self) {
  CogStringFree(self->name);
  free(self);
}

void CogCameraSetFov(CogCamera self, float fov) {
  self->fov = fov;
  self->projection = self->isOrtho ? 
                            CogMatrix4Ortho(-self->width * 0.5f, self->width * 0.5f, self->height * 0.5f, -self->height * 0.5f, self->near, self->far) : 
                            CogMatrix4Perspective(self->fov, self->width / (self->height > 0.0f ? self->height : 1.0f), self->near, self->far);
}

void CogCameraSetNearFar(CogCamera self, float near, float far) {
  self->near = near;
  self->far = far;
  self->projection = self->isOrtho ? 
                            CogMatrix4Ortho(-self->width * 0.5f, self->width * 0.5f, self->height * 0.5f, -self->height * 0.5f, self->near, self->far) : 
                            CogMatrix4Perspective(self->fov, self->width / (self->height > 0.0f ? self->height : 1.0f), self->near, self->far);
}

void CogCameraSetSize(CogCamera self, float width, float height) {
  self->width = width;
  self->height = height;
  self->projection = self->isOrtho ? 
                            CogMatrix4Ortho(-self->width * 0.5f, self->width * 0.5f, self->height * 0.5f, -self->height * 0.5f, self->near, self->far) : 
                            CogMatrix4Perspective(self->fov, self->width / (self->height > 0.0f ? self->height : 1.0f), self->near, self->far);
}

void CogCameraSetOrtho(CogCamera self, Bool toggle) {
  self->isOrtho = toggle;
  self->projection = self->isOrtho ? 
                            CogMatrix4Ortho(-self->width * 0.5f, self->width * 0.5f, self->height * 0.5f, -self->height * 0.5f, self->near, self->far) : 
                            CogMatrix4Perspective(self->fov, self->width / (self->height > 0.0f ? self->height : 1.0f), self->near, self->far);
}