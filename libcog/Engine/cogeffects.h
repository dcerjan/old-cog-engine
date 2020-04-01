//
//  EffectGaussianBlur.h
//  Cog
//
//  Created by Domagoj Cerjan on 28/04/14.
//  Copyright (c) 2014 Cog Web. All rights reserved.
//

#ifndef __Cog_EffectGaussianBlur__
#define __Cog_EffectGaussianBlur__

#include "cogrendertarget.h"

enum CogRenderTargetUnit;
struct CogFrameBuffer;
struct CogRenderTarget;
struct CogTexture;
struct CogShader;
struct CogSurface;


/*******************/
/** Gaussian Blur **/
/*******************/
struct CogTexture* CogEffectGaussianBlurRGBA(
  const float radius, const int numSamples, const struct CogTexture* input, struct CogFrameBuffer* fbo, 
  const enum CogRenderTargetUnit out, const enum CogRenderTargetUnit pingpong
);
struct CogTexture* CogEffectGaussianBlurRGB(
  const float radius, const int numSamples, const struct CogTexture* input, struct CogFrameBuffer* fbo, 
  const enum CogRenderTargetUnit out, const enum CogRenderTargetUnit pingpong
);
struct CogTexture* CogEffectGaussianBlurRG(
  const float radius, const int numSamples, const struct CogTexture* input, struct CogFrameBuffer* fbo, 
  const enum CogRenderTargetUnit out, const enum CogRenderTargetUnit pingpong
);
struct CogTexture* CogEffectGaussianBlurR(
  const float radius, const int numSamples, const struct CogTexture* input, struct CogFrameBuffer* fbo, 
  const enum CogRenderTargetUnit out, const enum CogRenderTargetUnit pingpong
);

/*************************/
/** FreiChen EdgeDetect **/
/*************************/
struct CogTexture* CogEffectFreiChenDepth(
  const float lowerBound, const float upperBound, const struct CogTexture* input, struct CogFrameBuffer* fbo,
  const enum CogRenderTargetUnit out
);
struct CogTexture* CogEffectFreiChenDepthNormal(
  const float lowerBound, const float upperBound, const struct CogTexture* inputDepth, const struct CogTexture* inputNormal, 
  struct CogFrameBuffer* fbo, const enum CogRenderTargetUnit out
);

/**********/
/** SSAO **/
/**********/
struct CogTexture* CogEffectSSAO(
  const float occluderBias, const int numSamples, const float samplingRadius, const float constantAttenuation,
  const float linearAttenuation, const float quadraticAttenuation, const struct CogTexture* position, 
  const struct CogTexture* normal, struct CogFrameBuffer* fbo, const enum CogRenderTargetUnit out
);

#endif
