#ifndef _COG_RENDERER_FORWARD_H_
#define _COG_RENDERER_FORWARD_H_

#include "cogrenderer.h"
#include "cogtypes.h"

struct CogFrameBuffer;
struct CogRenderTarget;
struct CogRendererForward {
  CogInherit(struct CogRenderer);
};
typedef struct CogRendererForward* CogRendererForward;
CogRenderer  CogRendererForwardAlloc   (void);

#endif