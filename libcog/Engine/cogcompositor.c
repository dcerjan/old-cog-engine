#include "cogcompositor.h"
#include "coggl.h"
#include "cogplatform.h"
#include "coglogger.h"
#include "cogshader.h"
#include "cogtexture.h"
#include "cogsurface.h"
#include "cogmesh.h"
#include "cogframebuffer.h"
#include "cogrendersystem.h"
#include "cogrenderer.h"

void CogCompositorCompose(
  const struct CogSurface* surface, 
  const struct CogShader* shader, 
  const struct CogTexture* textureChannel[], const int nTextures, 
  const float floatChannel[], const int nFloats,
  const int intChannel[], const int nInts
) {
  CogRenderer rnd = CogRenderSystemGetRenderer();
  rnd->bindShader(rnd, NULL, shader);
  int i;
  for(i = 0; i < 16; ++i) {
    if(i < nTextures && textureChannel[i]) {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(textureChannel[i]->type, textureChannel[i]->id);
      glUniform1i(shader->uniforms.fragment.compositor.texChannel[i], i);
    }
    if(i < nFloats) {
      glUniform1f(shader->uniforms.fragment.compositor.floatChannel[i], floatChannel[i]);
    }
    if(i < nInts) {
      glUniform1i(shader->uniforms.fragment.compositor.intChannel[i], intChannel[i]);
    }
  }
  CogSurfaceBlit((const CogSurface)surface, NULL, shader);
}

void CogCompositorComposeToRenderTarget(
  const struct CogSurface* surface, 
  const struct CogShader* shader, 
  const struct CogTexture* textureChannel[], const int nTextures, 
  const float floatChannel[], const int nFloats,
  const int intChannel[], const int nInts,
  const struct CogFrameBuffer* fbo,
  const CogRenderTargetUnit renderTarget
) {
  CogRenderer rnd = CogRenderSystemGetRenderer();
  rnd->bindShader(rnd, NULL, shader);
  int i;
  for(i = 0; i < 16; ++i) {
    if(i < nTextures && textureChannel[i]) {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(textureChannel[i]->type, textureChannel[i]->id);
      glUniform1i(shader->uniforms.fragment.compositor.texChannel[i], i);
    }
    if(i < nFloats) {
      glUniform1f(shader->uniforms.fragment.compositor.floatChannel[i], floatChannel[i]);
    }
    if(i < nInts) {
      glUniform1i(shader->uniforms.fragment.compositor.intChannel[i], intChannel[i]);
    }
  }
  
  CogSurfaceBlitToRenderTarget((const CogSurface)surface, NULL, shader, fbo, renderTarget);
}