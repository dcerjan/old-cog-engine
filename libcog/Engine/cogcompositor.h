#ifndef _COG_COMPOSITOR_H_
#define _COG_COMPOSITOR_H_

struct CogTexture;
struct CogShader;
struct CogSurface;
struct CogFrameBuffer;
enum CogRenderTargetUnit;

void CogCompositorCompose(
  const struct CogSurface* surface, 
  const struct CogShader* shader, 
  const struct CogTexture* textureChannel[], const int nTextures, 
  const float floatChannel[], const int nFloats,
  const int intChannel[], const int nInts
);

void CogCompositorComposeToRenderTarget(
  const struct CogSurface* surface, 
  const struct CogShader* shader, 
  const struct CogTexture* textureChannel[], const int nTextures, 
  const float floatChannel[], const int nFloats,
  const int intChannel[], const int nInts,
  const struct CogFrameBuffer* fbo,
  const enum CogRenderTargetUnit renderTarget
);

#endif