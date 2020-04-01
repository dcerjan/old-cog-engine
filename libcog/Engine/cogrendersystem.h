#ifndef _COG_RENDERSYSTEM_H_
#define _COG_RENDERSYSTEM_H_

#include "cogmatrix4.h"

struct CogRenderer;

struct CogRenderSystem {
  struct CogRenderer* activeRenderer;
  
  CogMatrix4  model;
  CogMatrix4  view;
  CogMatrix4  projection;

  float       width;
  float       height;
};

void
CogRenderSystemInit(
  void
);

void
CogRenderSystemSetRenderer(
  struct CogRenderer* renderer
);

struct CogRenderer*
CogRenderSystemGetRenderer(
  void
);

struct CogRenderSystem*
CogRenderSystemGet(
  void
);

#endif