#ifndef _COG_RENDER_TARGET_H_
#define _COG_RENDER_TARGET_H_

#include "cogtypes.h"
#include "cogtexture.h"

typedef enum CogRenderTargetUnit {
  CogRenderTargetUnit0  =  GL_COLOR_ATTACHMENT0,
  CogRenderTargetUnit1  =  GL_COLOR_ATTACHMENT1,
  CogRenderTargetUnit2  =  GL_COLOR_ATTACHMENT2,
  CogRenderTargetUnit3  =  GL_COLOR_ATTACHMENT3,
  CogRenderTargetUnit4  =  GL_COLOR_ATTACHMENT4,
  CogRenderTargetUnit5  =  GL_COLOR_ATTACHMENT5,
  CogRenderTargetUnit6  =  GL_COLOR_ATTACHMENT6,
  CogRenderTargetUnit7  =  GL_COLOR_ATTACHMENT7,
  CogRenderTargetUnit8  =  GL_COLOR_ATTACHMENT8,
  CogRenderTargetUnit9  =  GL_COLOR_ATTACHMENT9,
  CogRenderTargetUnit10 =  GL_COLOR_ATTACHMENT10,
  CogRenderTargetUnit11 =  GL_COLOR_ATTACHMENT11,
  CogRenderTargetUnit12 =  GL_COLOR_ATTACHMENT12,
  CogRenderTargetUnit13 =  GL_COLOR_ATTACHMENT13,
  CogRenderTargetUnit14 =  GL_COLOR_ATTACHMENT14,
  CogRenderTargetUnit15 =  GL_COLOR_ATTACHMENT15
} CogRenderTargetUnit;

struct CogFrameBuffer;
struct CogRenderTarget {
  CogString                 name;
  CogTexture                texture;
  CogRenderTargetUnit       unit;
  struct CogFrameBuffer*    fbo;
};
typedef struct CogRenderTarget* CogRenderTarget;



#endif