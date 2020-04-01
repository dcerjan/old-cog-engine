//
//  Effect.c
//  Cog
//
//  Created by Domagoj Cerjan on 28/04/14.
//  Copyright (c) 2014 Cog Web. All rights reserved.
//

#include "cogeffects.h"

#include "cogtexture.h"
#include "cogshader.h"
#include "cogframebuffer.h"
#include "cogrendertarget.h"
#include "cogrenderer.h"
#include "cogrendererdeferred.h"
#include "cogrendersystem.h"
#include "cogsurface.h"

/*******************/
/** Gaussian Blur **/
/*******************/
struct CogTexture* CogEffectGaussianBlurRGBA(
  const float radius, 
  const int numSamples, 
  const struct CogTexture* input, 
  struct CogFrameBuffer* fbo, 
  const CogRenderTargetUnit out, 
  const CogRenderTargetUnit pingpong
) {
  CogRendererDeferred r = (CogRendererDeferred)CogRenderSystemGetRenderer();
  
  r->super.bindShader((CogRenderer)r, NULL, r->shaders.util.blur.gaussian.rgba);
  
  glUniform1i(r->shaders.util.blur.gaussian.rgba->uniforms.fragment.compositor.intChannel[1], 1);
  glUniform1i(r->shaders.util.blur.gaussian.rgba->uniforms.fragment.compositor.intChannel[0], numSamples);
  glUniform1f(r->shaders.util.blur.gaussian.rgba->uniforms.fragment.compositor.floatChannel[0], radius);
  CogSurfaceBlitToRenderTarget(r->surface, input, r->shaders.util.blur.gaussian.rgba, fbo, pingpong);
  
  glUniform1i(r->shaders.util.blur.gaussian.rgba->uniforms.fragment.compositor.intChannel[1], 0);
  glUniform1i(r->shaders.util.blur.gaussian.rgba->uniforms.fragment.compositor.intChannel[0], numSamples);
  glUniform1f(r->shaders.util.blur.gaussian.rgba->uniforms.fragment.compositor.floatChannel[0], radius);
  CogSurfaceBlitToRenderTarget(r->surface, fbo->renderTargets[pingpong - CogRenderTargetUnit0]->texture, r->shaders.util.blur.gaussian.rgba, fbo, out);
  
  return fbo->renderTargets[out - CogRenderTargetUnit0]->texture;
}

struct CogTexture* CogEffectGaussianBlurRGB(
  const float radius, const int numSamples, const struct CogTexture* input, struct CogFrameBuffer* fbo, 
  const CogRenderTargetUnit out, const CogRenderTargetUnit pingpong
) {
  CogRendererDeferred r = (CogRendererDeferred)CogRenderSystemGetRenderer();
  
  r->super.bindShader((CogRenderer)r, NULL, r->shaders.util.blur.gaussian.rgb);
  glUniform1i(r->shaders.util.blur.gaussian.rgb->uniforms.fragment.compositor.intChannel[0], numSamples);
  glUniform1i(r->shaders.util.blur.gaussian.rgb->uniforms.fragment.compositor.intChannel[1], 0);
  glUniform1f(r->shaders.util.blur.gaussian.rgb->uniforms.fragment.compositor.floatChannel[0], radius);
  CogSurfaceBlitToRenderTarget(r->surface, input, r->shaders.util.blur.gaussian.rgb, fbo, pingpong);
  
  glUniform1i(r->shaders.util.blur.gaussian.rgb->uniforms.fragment.compositor.intChannel[0], numSamples);
  glUniform1i(r->shaders.util.blur.gaussian.rgb->uniforms.fragment.compositor.intChannel[1], 1);
  glUniform1f(r->shaders.util.blur.gaussian.rgb->uniforms.fragment.compositor.floatChannel[0], radius);
  CogSurfaceBlitToRenderTarget(r->surface, fbo->renderTargets[pingpong - CogRenderTargetUnit0]->texture, r->shaders.util.blur.gaussian.rgb, fbo, out);
  
  return fbo->renderTargets[out - CogRenderTargetUnit0]->texture;
}
struct CogTexture* CogEffectGaussianBlurRG(
  const float radius, const int numSamples, const struct CogTexture* input, struct CogFrameBuffer* fbo, 
  const CogRenderTargetUnit out, const CogRenderTargetUnit pingpong
) {
  CogRendererDeferred r = (CogRendererDeferred)CogRenderSystemGetRenderer();
  
  r->super.bindShader((CogRenderer)r, NULL, r->shaders.util.blur.gaussian.rg);
  glUniform1i(r->shaders.util.blur.gaussian.rg->uniforms.fragment.compositor.intChannel[0], numSamples);
  glUniform1i(r->shaders.util.blur.gaussian.rg->uniforms.fragment.compositor.intChannel[1], 0);
  glUniform1f(r->shaders.util.blur.gaussian.rg->uniforms.fragment.compositor.floatChannel[0], radius);
  CogSurfaceBlitToRenderTarget(r->surface, input, r->shaders.util.blur.gaussian.rg, fbo, pingpong);
  
  glUniform1i(r->shaders.util.blur.gaussian.rg->uniforms.fragment.compositor.intChannel[0], numSamples);
  glUniform1i(r->shaders.util.blur.gaussian.rg->uniforms.fragment.compositor.intChannel[1], 1);
  glUniform1f(r->shaders.util.blur.gaussian.rg->uniforms.fragment.compositor.floatChannel[0], radius);
  CogSurfaceBlitToRenderTarget(r->surface, fbo->renderTargets[pingpong - CogRenderTargetUnit0]->texture, r->shaders.util.blur.gaussian.rg, fbo, out);
  
  return fbo->renderTargets[out - CogRenderTargetUnit0]->texture;
}
struct CogTexture* CogEffectGaussianBlurR(
  const float radius, const int numSamples, const struct CogTexture* input, struct CogFrameBuffer* fbo, 
  const CogRenderTargetUnit out, const CogRenderTargetUnit pingpong
) {
  CogRendererDeferred r = (CogRendererDeferred)CogRenderSystemGetRenderer();
  
  r->super.bindShader((CogRenderer)r, NULL, r->shaders.util.blur.gaussian.r);
  glUniform1i(r->shaders.util.blur.gaussian.r->uniforms.fragment.compositor.intChannel[0], numSamples);
  glUniform1i(r->shaders.util.blur.gaussian.r->uniforms.fragment.compositor.intChannel[1], 0);
  glUniform1f(r->shaders.util.blur.gaussian.r->uniforms.fragment.compositor.floatChannel[0], radius);
  CogSurfaceBlitToRenderTarget(r->surface, input, r->shaders.util.blur.gaussian.r, fbo, pingpong);
  
  glUniform1i(r->shaders.util.blur.gaussian.r->uniforms.fragment.compositor.intChannel[0], numSamples);
  glUniform1i(r->shaders.util.blur.gaussian.r->uniforms.fragment.compositor.intChannel[1], 1);
  glUniform1f(r->shaders.util.blur.gaussian.r->uniforms.fragment.compositor.floatChannel[0], radius);
  CogSurfaceBlitToRenderTarget(r->surface, fbo->renderTargets[pingpong - CogRenderTargetUnit0]->texture, r->shaders.util.blur.gaussian.r, fbo, out);
  
  return fbo->renderTargets[out - CogRenderTargetUnit0]->texture;
}


/*************************/
/** FreiChen EdgeDetect **/
/*************************/
struct CogTexture* CogEffectFreiChenDepth(
  const float lowerBound, const float upperBound, const struct CogTexture* input, struct CogFrameBuffer* fbo,
  const enum CogRenderTargetUnit out
) {
  CogRendererDeferred r = (CogRendererDeferred)CogRenderSystemGetRenderer();
  
  r->super.bindShader((CogRenderer)r, NULL, r->shaders.util.edgeDetect.freiChen.depth);
  glUniform1f(r->shaders.util.edgeDetect.freiChen.depth->uniforms.fragment.compositor.floatChannel[0], lowerBound);
  glUniform1f(r->shaders.util.edgeDetect.freiChen.depth->uniforms.fragment.compositor.floatChannel[1], upperBound);
  CogSurfaceBlitToRenderTarget(r->surface, input, r->shaders.util.edgeDetect.freiChen.depth, fbo, out);
  
  return fbo->renderTargets[out - CogRenderTargetUnit0]->texture;
}
struct CogTexture* CogEffectFreiChenDepthNormal(
  const float lowerBound, const float upperBound, const struct CogTexture* inputDepth, const struct CogTexture* inputNormal,
  struct CogFrameBuffer* fbo, const enum CogRenderTargetUnit out
) {
  CogRendererDeferred r = (CogRendererDeferred)CogRenderSystemGetRenderer();
  
  r->super.bindShader((CogRenderer)r, NULL, r->shaders.util.edgeDetect.freiChen.depthNormal);
  glUniform1f(r->shaders.util.edgeDetect.freiChen.depthNormal->uniforms.fragment.compositor.floatChannel[0], lowerBound);
  glUniform1f(r->shaders.util.edgeDetect.freiChen.depthNormal->uniforms.fragment.compositor.floatChannel[1], upperBound);
  CogSurfaceBlitMultipleToRenderTarget(
    r->surface, 2,
    (const struct CogTexture**)(CogTexture[]){
      inputDepth,
      inputNormal
    },
    r->shaders.util.edgeDetect.freiChen.depthNormal,
    fbo,
    out
  );
  
  return fbo->renderTargets[out - CogRenderTargetUnit0]->texture;
}

/**********/
/** SSAO **/
/**********/
struct CogTexture* CogEffectSSAO(
  const float occluderBias, const int numSamples, const float samplingRadius, const float constantAttenuation,
  const float linearAttenuation, const float quadraticAttenuation, const struct CogTexture* position, 
  const struct CogTexture* normal, struct CogFrameBuffer* fbo, const enum CogRenderTargetUnit out
) {
  CogRendererDeferred r = (CogRendererDeferred)CogRenderSystemGetRenderer();
  
  r->super.bindShader((CogRenderer)r, NULL, r->shaders.effect.ambientOcclusion);
  glUniform1f(r->shaders.effect.ambientOcclusion->uniforms.fragment.compositor.floatChannel[0], occluderBias); // Occluder bias
  glUniform1f(r->shaders.effect.ambientOcclusion->uniforms.fragment.compositor.floatChannel[1], samplingRadius); // Sampling Radius
  glUniform1f(r->shaders.effect.ambientOcclusion->uniforms.fragment.compositor.floatChannel[2], constantAttenuation); // Constant Attenuation
  glUniform1f(r->shaders.effect.ambientOcclusion->uniforms.fragment.compositor.floatChannel[3], linearAttenuation); // Linear Attenuation
  glUniform1f(r->shaders.effect.ambientOcclusion->uniforms.fragment.compositor.floatChannel[4], quadraticAttenuation); // Quadratic Attenuation
  glUniform1i(r->shaders.effect.ambientOcclusion->uniforms.fragment.compositor.intChannel[0], numSamples); // NumSamples
  CogSurfaceBlitMultipleToRenderTarget(
    r->surface, 3,
    (const struct CogTexture**)(CogTexture[]){
      position,
      normal,
      r->randommap
    },
    r->shaders.effect.ambientOcclusion,
    fbo,
    out
  );
  
  return fbo->renderTargets[out - CogRenderTargetUnit0]->texture;
}