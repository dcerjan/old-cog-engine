#include "cogframebuffer.h"
#include "cogplatform.h"
#include "cogresourcemanager.h"

Bool CogFrameBufferCheck(CogFrameBuffer self) {
  glBindFramebuffer(GL_FRAMEBUFFER, self->id);
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch(status) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      CogLoggerError(CogPlatformGetLogger(), "Framebuffer %s has incomplete attachments.\n", c_str(self->name));
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      return False;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      CogLoggerError(CogPlatformGetLogger(), "Framebuffer %s has incomplete draw buffer.\n", c_str(self->name));
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      return False;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
      CogLoggerError(CogPlatformGetLogger(), "Framebuffer %s has incomplete layer targets.\n", c_str(self->name));
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      return False;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      CogLoggerError(CogPlatformGetLogger(), "Framebuffer %s has a missing attachment.\n", c_str(self->name));
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      return False;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
      CogLoggerError(CogPlatformGetLogger(), "Framebuffer %s has incomplete multisampling.\n", c_str(self->name));
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      return False;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      CogLoggerError(CogPlatformGetLogger(), "Framebuffer %s has incomplete read buffer.\n", c_str(self->name));
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      return False;
      break;
    case GL_FRAMEBUFFER_COMPLETE:
      CogLoggerInfo(CogPlatformGetLogger(), "Framebuffer %s is complete.\n", c_str(self->name));
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      return True;
      break;
    default:
      return False;
  }
  return False;
}

CogFrameBuffer CogFrameBufferAlloc(const char* name, uint32 width, uint32 height) {
  CogFrameBuffer self = malloc(sizeof(struct CogFrameBuffer));
  
  self->name = CogStringAlloc(name);
  self->width = width;
  self->height = height;
  
  self->clearFlag = GL_COLOR_BUFFER_BIT;
  self->depthBuffer = False;
  self->stencilBuffer = False;
  
  memset(self->renderTargets, 0, sizeof(CogRenderTarget) * 16);
  self->numRenderTargets = 0;
  
  glGenFramebuffers(1, &self->id);
  glBindFramebuffer(GL_FRAMEBUFFER, self->id);
  glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &self->maxNumRenderTargets);
  
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  
  glBindFramebuffer(GL_FRAMEBUFFER, self->id);
  
  return self;
}

void CogFrameBufferEnableDepthBuffer(CogFrameBuffer self, float clear) {
  if(self->depthBuffer) {
    CogLoggerWarning(CogPlatformGetLogger(), "Frame Buffer '%s' already has a depth buffer attached.\n", c_str(self->name));
    return;
  }
  self->depthBuffer = True;
  self->clearFlag |= GL_DEPTH_BUFFER_BIT;
  self->depthClear = clear;
  
  glBindFramebuffer(GL_FRAMEBUFFER, self->id);
  
  glClearDepth(self->depthClear);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  glGenRenderbuffers(1, &self->depthId);
  
  glClearDepth(self->depthClear);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  
  glBindRenderbuffer(GL_RENDERBUFFER, self->depthId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, self->width, self->height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, self->depthId);
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CogFrameBufferEnableStencilBuffer(CogFrameBuffer self, unsigned char clear) {
  if(self->stencilBuffer) {
    CogLoggerWarning(CogPlatformGetLogger(), "Frame Buffer '%s' already has a stencil buffer attached.\n", c_str(self->name));
    return;
  }
  self->stencilBuffer = True;
  self->stencilClear = clear;
  self->clearFlag |= GL_STENCIL_BUFFER_BIT;
  
  glBindFramebuffer(GL_FRAMEBUFFER, self->id);
  glGenRenderbuffers(1, &self->depthId);
  
  glBindRenderbuffer(GL_RENDERBUFFER, self->depthId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, self->width, self->height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, self->stencilId);
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CogFrameBufferAttachRenderTarget(CogFrameBuffer self, const char* name, CogColorFormat format, CogColorInternalFormat internal, CogRenderTargetUnit unit) {
  assert( (unit - CogRenderTargetUnit0) >= 0 && ((unit - CogRenderTargetUnit0) < self->maxNumRenderTargets));
  if(self->renderTargets[unit - CogRenderTargetUnit0] != 0) {
    CogLoggerWarning(CogPlatformGetLogger(), "Frame Buffer '%s' already has a render target unit %d attached.\n", c_str(self->name), unit - CogRenderTargetUnit0);
    return;
  }
  
  self->numRenderTargets++;

  CogRenderTarget rt = malloc(sizeof(struct CogRenderTarget));
  
  rt->name = CogStringAlloc(name);
  rt->unit = unit;
  rt->texture = (CogTexture)malloc(sizeof(struct CogTexture));
  rt->texture->name = CogStringAlloc(name);
  rt->texture->format = format;
  rt->texture->internalFormat = internal;
  glGenTextures(1, &rt->texture->id);
  rt->texture->anisotropicFiltering = False;
  rt->texture->anisotropyLevel = 1.0f;
  rt->texture->mipmaps = False;
  rt->texture->type = CogTextureTypeTexture2D;
  rt->texture->min = CogTextureFilteringLinear;
  rt->texture->mag = CogTextureFilteringLinear;
  rt->texture->width = self->width;
  rt->texture->height = self->height;
  rt->fbo = self;
  
  self->renderTargets[unit - CogRenderTargetUnit0] = rt;
  
  glBindFramebuffer(GL_FRAMEBUFFER, self->id);
  
  glBindTexture(GL_TEXTURE_2D, rt->texture->id);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, rt->texture->mag);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, rt->texture->min);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, internal, self->width, self->height, 0, format, CogTextureGetComponentType(rt->texture), 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, unit, rt->texture->type, rt->texture->id, 0);
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

/*
void FrameBufferAttachMultiSamplingRenderTarget(FrameBuffer self, const char* name, int samples, CogColorInternalFormat internal, RenderTargetUnit unit) {
  assert( (unit - RenderTargetUnit0) >= 0 && ((unit - RenderTargetUnit0) < self->maxNumRenderTargets));
  if(self->renderTargets[unit - RenderTargetUnit0] != 0) {
    LoggerWarning(PlatformGetLogger(), "Frame Buffer '%s' already has a render target unit %d attached.\n", c_str(self->name), unit - RenderTargetUnit0);
    return;
  }
  
  RenderTarget rt = malloc(sizeof(struct RenderTarget));
  
  rt->name = CogStringAlloc(name);
  rt->texture = (Texture)malloc(sizeof(struct Texture));
  rt->texture->name = CogStringAlloc(name);
  rt->texture->internalFormat = internal;
  glGenTextures(1, &rt->texture->id);
  rt->texture->anisotropicFiltering = False;
  rt->texture->anisotropyLevel = 1.0f;
  rt->texture->mipmaps = False;
  rt->texture->type = Texture2DMultiSample;
  rt->texture->min = Nearest;
  rt->texture->mag = Nearest;
  rt->texture->width = self->width;
  rt->texture->height = self->height;
  
  self->renderTargets[unit - RenderTargetUnit0] = rt;
  
  glBindFramebuffer(GL_FRAMEBUFFER, self->id);
  
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, rt->texture->id);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexCogImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal, self->width, self->height, GL_FALSE);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, unit, rt->texture->type, rt->texture->id, 0);
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
*/

void CogFrameBufferBegin(CogFrameBuffer self, CogRenderTargetUnit unit) {
  glBindFramebuffer(GL_FRAMEBUFFER, self->id);
  glViewport(0, 0, self->width, self->height);
  glDrawBuffer(unit);
  glClear(self->clearFlag);
}

void CogFrameBufferBeginMultiple(CogFrameBuffer self, int nUnits, CogRenderTargetUnit units[]) {
  glBindFramebuffer(GL_FRAMEBUFFER, self->id);
  glViewport(0, 0, self->width, self->height);
  glDrawBuffers(nUnits, (const GLenum*)units);
  glClear(self->clearFlag);
}

void CogFrameBufferEnd(void) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  CogRect wndCogRect = CogPlatformGetFocusedWindow()->rect;
  glViewport(0, 0, wndCogRect.w, wndCogRect.h);
}

void CogFrameBufferFree(CogFrameBuffer self) {
  CogFrameBufferEnd();
  
  if(self->depthBuffer) {
    glDeleteRenderbuffers(1, &self->depthId);
  }
  
  if(self->stencilBuffer) {
    glDeleteRenderbuffers(1, &self->stencilId);
  }
  
  int i;
  for(i = 0; i < self->maxNumRenderTargets; ++i) {
    if(self->renderTargets[i]) {
      CogStringFree(self->renderTargets[i]->name);
      CogTextureFree(self->renderTargets[i]->texture);
      free(self->renderTargets[i]);
      self->renderTargets[i] = NULL;
    }
  }
  
  glDeleteFramebuffers(1, &self->id);
  
  CogStringFree(self->name);
  free(self);
}

void CogFrameBufferInspect(CogFrameBuffer self) {
  
}

void CogFrameBufferClear(CogFrameBuffer self) {
  glClear(self->clearFlag);
}
