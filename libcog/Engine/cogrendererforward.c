#include "cogrendererforward.h"
#include "cogscene.h"
#include "cogtexture.h"
#include "cogshader.h"
#include "cogmaterial.h"
#include "cogmesh.h"
#include "cogmodel.h"
#include "cogframebuffer.h"
#include "cogrendertarget.h"
#include "cogscenegraph.h"
#include "cognode.h"
#include "cogcamera.h"
#include "CogVector3.h"
#include "cogmatrix4.h"


void _bindShader(CogShader self) { 
  glUseProgram(self->id); 
}

void _unbindShader(void) { 
  glUseProgram(0); 
}

void _bindMaterial(const CogMaterial self) {
  _bindShader(self->shader);
  
  
}

void _unbindMaterial(const CogMaterial self) {
  _unbindShader();
}

void _bindCogMesh(const CogMesh self, CogShader shader) {
  
}

void _bindCogNode(CogNode self, CogShader shader) {
  
}

void _bindCogCamera(CogCamera self, CogShader shader) {
  
}

void _traverseCogNodes(CogNode self) {
  CogListForEach(self->children, child, {
    
  });
}

void _rendererForwardRender(CogRenderer self, CogScene scene) {
  
}

void _rendererForwardFree(CogRenderer self) {
  
}


CogRenderer CogRendererForwardAlloc(void) {
  CogRendererForward self = malloc(sizeof(struct CogRendererForward));
  
  self->super.name = CogStringAlloc("Default Forward Renderer");
  
  /* TODO */
  //self->super.renderScene = _rendererForwardRender;
  //self->super.free = _rendererForwardFree;
  
  return (CogRenderer)self;
}

