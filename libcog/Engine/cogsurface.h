#ifndef _COG_SURFACE_H_
#define _COG_SURFACE_H_

#include "cogrect.h"
#include "cogstring.h"

struct CogCompositor;
struct CogMesh;
struct CogShader;
struct CogTexture;
struct CogFrameBuffer;
enum CogRenderTargetUnit;
struct CogSurface {
  CogString                       name;
  CogRect                         rect;
  struct CogMesh*                 quad;
};
typedef struct CogSurface* CogSurface;

CogSurface
CogSurfaceAlloc(
  const char*                     name,
  const CogRect                   rect
);

void
CogSurfaceInspect(
  const CogSurface                self
);

void
CogSurfaceFree(
  CogSurface                      self
);

void
CogSurfaceBlit(
  const CogSurface                self, 
  const struct CogTexture*        texture, 
  const struct CogShader*         shader
);

void
CogSurfaceBlitMultiple(
  const CogSurface                self, 
  const int                       nTextures, 
  const struct CogTexture*        texture[], 
  const struct CogShader*         shader
);

void
CogSurfaceBlitToRenderTarget(
  const CogSurface                self, 
  const struct CogTexture*        texture, 
  const struct CogShader*         shader, 
  const struct CogFrameBuffer*    fbo, 
  const enum CogRenderTargetUnit  rt
);

void
CogSurfaceBlitMultipleToRenderTarget(
  const CogSurface                self,
  const int                       nTextures,
  const struct CogTexture*        texture[],
  const struct CogShader*         shader,
  const struct CogFrameBuffer*    fbo,
  const enum CogRenderTargetUnit  rt
);

#endif