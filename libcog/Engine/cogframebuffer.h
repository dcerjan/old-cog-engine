#ifndef _COG_FRAME_BUFFER_H_
#define _COG_FRAME_BUFFER_H_

#include "coggl.h"
#include "cogtypes.h"
#include "cogrendertarget.h"
#include "cogcolor.h"

struct CogFrameBuffer {
  CogString           name;
  Bool                depthBuffer;
  float               depthClear;
  GLuint              depthId;
  Bool                stencilBuffer;
  unsigned char       stencilClear;
  GLuint              stencilId;
  int                 numRenderTargets;
  int                 maxNumRenderTargets;
  CogRenderTarget     renderTargets[16];
  uint32              clearFlag;
  uint32              width;
  uint32              height;
  GLuint              id;
};
typedef struct CogFrameBuffer* CogFrameBuffer;

CogFrameBuffer    CogFrameBufferAlloc                            (const char* name, uint32 width, uint32 height);
void              CogFrameBufferEnableDepthBuffer                (CogFrameBuffer self, float clear);
void              CogFrameBufferEnableStencilBuffer              (CogFrameBuffer self, unsigned char clear);
void              CogFrameBufferAttachRenderTarget               (CogFrameBuffer self, const char* name, CogColorFormat format, CogColorInternalFormat internal, CogRenderTargetUnit unit);
//void              CogFrameBufferAttachMultiSamplingRenderTarget  (CogFrameBuffer self, const char* name, int samples, CogColorInternalFormat internal, CogRenderTargetUnit unit);
void              CogFrameBufferBegin                            (CogFrameBuffer self, CogRenderTargetUnit unit);
void              CogFrameBufferBeginMultiple                    (CogFrameBuffer self, int nUnits, CogRenderTargetUnit units[]);
void              CogFrameBufferEnd                              (void);
void              CogFrameBufferFree                             (CogFrameBuffer self);
void              CogFrameBufferInspect                          (CogFrameBuffer self);
Bool              CogFrameBufferCheck                            (CogFrameBuffer self);
void              CogFrameBufferClear                            (CogFrameBuffer self);

#endif