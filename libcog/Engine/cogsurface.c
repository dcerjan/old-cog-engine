#include "cogsurface.h"
#include "cogmesh.h"
#include "cogtexture.h"
#include "cogshader.h"
#include "cogcompositor.h"
#include "cogframebuffer.h"
#include "cogrendersystem.h"
#include "cogrenderer.h"

CogSurface CogSurfaceAlloc(const char* name, const CogRect rect) {
  CogSurface self = malloc(sizeof(struct CogSurface));
  
  self->name = CogStringAlloc(name);
  
  self->rect = rect;
  
  self->quad = CogMeshAlloc(name, NULL);
  self->quad->drawMethod = CogMeshDrawMethodTriangleFan;
  self->quad->drawUsage = CogMeshUsageStaticDraw;
  
  // positions
  CogMeshAddBuffer(self->quad, Component2D);
  CogMeshAddComponent2(self->quad, 0, (CogVector2){-1.0f + 2.0f * rect.x           , 1.0f - 2.0f * (rect.y)         });
  CogMeshAddComponent2(self->quad, 0, (CogVector2){-1.0f + 2.0f * (rect.x + rect.w), 1.0f - 2.0f * (rect.y)         });
  CogMeshAddComponent2(self->quad, 0, (CogVector2){-1.0f + 2.0f * (rect.x + rect.w), 1.0f - 2.0f * (rect.y + rect.h)});
  CogMeshAddComponent2(self->quad, 0, (CogVector2){-1.0f + 2.0f * rect.x           , 1.0f - 2.0f * (rect.y + rect.h)});
  
  // texcoords
  CogMeshAddBuffer(self->quad, Component2D);
  CogMeshAddComponent2(self->quad, 1, (CogVector2){0.0f, 0.0f});
  CogMeshAddComponent2(self->quad, 1, (CogVector2){1.0f, 0.0f});
  CogMeshAddComponent2(self->quad, 1, (CogVector2){1.0f, 1.0f});
  CogMeshAddComponent2(self->quad, 1, (CogVector2){0.0f, 1.0f});
  
  CogMeshCompile(self->quad);
  
  return self;
}

void CogSurfaceInspect(const CogSurface self) {
  
}

void CogSurfaceFree(CogSurface self) {
  CogStringFree(self->name);
  CogMeshFree(self->quad);
}

void CogSurfaceBlit (const CogSurface self, const struct CogTexture* texture, const struct CogShader* shader) {
  if(texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(texture->type, texture->id);
    glUniform1i(shader->uniforms.fragment.compositor.texChannel[0], 0);
  }
  CogRenderer r = CogRenderSystemGetRenderer();
  r->bindCogMesh(r, NULL, (const CogShader)shader, (const CogMesh)self->quad);
}

void CogSurfaceBlitMultiple (const CogSurface self, const int nTextures, const struct CogTexture* texture[], const struct CogShader* shader) {
  int i;
  for(i = 0; i < nTextures; ++i) {
    if(texture[i]) {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(texture[i]->type, texture[i]->id);
      glUniform1i(shader->uniforms.fragment.compositor.texChannel[i], i);
    }
  }
  CogRenderer r = CogRenderSystemGetRenderer();
  r->bindCogMesh(r, NULL, (const CogShader)shader, (const CogMesh)self->quad);
}

void CogSurfaceBlitToRenderTarget(const CogSurface self, const struct CogTexture* texture, const struct CogShader* shader, const struct CogFrameBuffer* fbo, CogRenderTargetUnit rt) {
  if(texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(texture->type, texture->id);
    glUniform1i(shader->uniforms.fragment.compositor.texChannel[0], 0);
  }
  CogFrameBufferBegin((const CogFrameBuffer)fbo, rt);
  CogRenderer r = CogRenderSystemGetRenderer();
  r->bindCogMesh(r, NULL, (const CogShader)shader, (const CogMesh)self->quad);
  CogFrameBufferEnd();
}

void CogSurfaceBlitMultipleToRenderTarget(const CogSurface self, const int nTextures, const struct CogTexture* texture[], const struct CogShader* shader, const struct CogFrameBuffer* fbo, CogRenderTargetUnit rt) {
  int i;
  for(i = 0; i < nTextures; ++i) {
    if(texture[i]) {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(texture[i]->type, texture[i]->id);
      glUniform1i(shader->uniforms.fragment.compositor.texChannel[i], i);
    }
  }
  CogFrameBufferBegin((const CogFrameBuffer)fbo, rt);
  CogRenderer r = CogRenderSystemGetRenderer();
  r->bindCogMesh(r, NULL, (const CogShader)shader, (const CogMesh)self->quad);
  CogFrameBufferEnd();
}