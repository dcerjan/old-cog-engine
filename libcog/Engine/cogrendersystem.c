#include "cogrendersystem.h"
#include "cogrenderer.h"
#include "cogrendererdeferred.h"
#include "cogrendererforward.h"

#include "cogmatrix4.h"

// singleton
static struct CogRenderSystem __cog_render_system__;

void CogRenderSystemInit(void) {
  __cog_render_system__.activeRenderer = NULL;
}

void CogRenderSystemSetRenderer(struct CogRenderer* renderer) {
  __cog_render_system__.activeRenderer = renderer;
}

struct CogRenderer* CogRenderSystemGetRenderer(void) {
  return __cog_render_system__.activeRenderer;
}

struct CogRenderSystem* CogRenderSystemGet(void) {
  return &__cog_render_system__;
}