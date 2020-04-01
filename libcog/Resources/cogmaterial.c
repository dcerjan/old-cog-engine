#include "cogmaterial.h"
#include "cogbundle.h"
#include "coglogger.h"
#include "cogplatform.h"
#include "cogrendersystem.h"
#include "cogrenderer.h"

CogMaterial CogMaterialAlloc(const char* name, CogShader shader, struct CogBundle* bundle) {
  CogMaterial self = malloc(sizeof(struct CogMaterial));
  
  self->name = CogStringAlloc(name);
  self->shader = shader;
  
  
  self->ambient.ambientIntensity = 0.0f;
  self->ambient.ambientOcclusionStrenght = 0.0f;
  self->ambient.ambientColor = (CogColor){0.0f, 0.0f, 0.0f, 1.0f};
  self->ambient.ambientOcclusionMap = NULL;
  
  self->diffuse.diffuseIntensity = 0.0f;
  self->diffuse.diffuseColor = (CogColor){0.0f, 0.0f, 0.0f, 1.0f};
  self->diffuse.diffuseMap = NULL;
  
  self->specular.specularIntensity = 0.0f;
  self->specular.specularHardness = 1.0f;
  self->specular.specularColor = (CogColor){0.0f, 0.0f, 0.0f, 1.0f};
  self->specular.specularMap = NULL;
  
  self->bump.normalMap = NULL;
  self->bump.heightMap = NULL;
  self->bump.heightScale = 1.0f;
  
  self->glow.glowStrenght = 0.0f;
  self->glow.glowColor = (CogColor){0.0f, 0.0f, 0.0f, 1.0f};
  self->glow.glowMap = NULL;
  
  self->detail.detailStrenght = 0.0f;
  self->detail.detailMap = NULL;
  
  self->reflection.reflectionStrenght = 1.0f;
  self->reflection.reflectionMap = NULL;
  
  if(bundle) {
    CogBundleAddMaterial(bundle, self);
  }
  
  CogLoggerInfo(CogPlatformGetLogger(), "[Resource] Material <%s> created.\n", self->name->data);
  return self;
}

void CogMaterialInspect(const CogMaterial self) {
  
}

void CogMaterialFree(CogMaterial self) {
  CogLoggerInfo(CogPlatformGetLogger(), "[Resource] Material <%s> deleted.\n", self->name->data);
  CogStringFree(self->name);
  free(self);
}