#include "coglight.h"
#include "cogbundle.h"
#include "coglogger.h"
#include "cogplatform.h"
#include "cognode.h"
#include "cogscenemanager.h"
#include "cogscene.h"

void CogLightInspect(const CogLight self) {
  self->inspect(self);
}

void CogLightFree(CogLight self) {
  CogLoggerInfo(CogPlatformGetLogger(), "[Resource] Light <%s> deleted.\n", self->name->data);
  self->free(self);
}

static void _setLight(CogLight self, struct CogScene* scene, struct CogNode* parent, const char* name) {
  
  self->name = CogStringAlloc(name);
  self->intensity = 1.0f;
  
  self->parent = parent;
  CogNodeAttachLight(parent, self);
  
  self->color = (CogColor){1.0f, 1.0f, 1.0f, 1.0f};
  self->projector = False;
  self->shadow = CogShadowTypeNone;
  self->shadowCascades = 1;
  
  self->free = NULL;
  self->inspect = NULL;
  
  CogArrayPush(scene->lights, self);
}




/////////////////////
//  Ambient Light  //
/////////////////////
static void _ambientLightInspect(const CogLight self) {
  printf("<Ambient Light '%s' @0x%ux>\n", self->name->data, (unsigned int)self);
}

static void _ambientLightFree(CogLight self) {
  CogAmbientLight s = (CogAmbientLight)self;
  CogStringFree(s->super.name);
  free(s);
}
CogAmbientLight CogAmbientLightAlloc(const char* name, struct CogScene* scene, struct CogNode* parent, const int unit, struct CogBundle* bundle) {
  CogAmbientLight self = malloc(sizeof(struct CogAmbientLight));
  _setLight((CogLight)self, scene, parent, name);
  
  self->super.type = CogLightTypeAmbient;
  self->super.color = (CogColor){1.0f, 1.0f, 1.0f, 1.0f};
  self->super.unit = unit;
  
  self->super.free = _ambientLightFree;
  self->super.inspect = _ambientLightInspect;
  
  if(bundle) {
    CogBundleAddLight(bundle, (CogLight)self);
  }
  
  return self;
}




///////////////////
//  Point Light  //
///////////////////
static void _pointLightInspect(const CogLight self) {
  printf("<Point Light '%s' @0x%ux>\n", self->name->data, (unsigned int)self);
}

static void _pointLightFree(CogLight self) {
  CogPointLight s = (CogPointLight)self;
  CogStringFree(s->super.name);
  free(s);
}
CogPointLight CogPointLightAlloc(const char* name, struct CogScene* scene, struct CogNode* parent, CogTexture projectedCubeMap, CogTexture shadowCubeMap, const int unit, struct CogBundle* bundle) {
  CogPointLight self = malloc(sizeof(struct CogPointLight));
  _setLight((CogLight)self, scene, parent, name);
  
  
  self->super.type = CogLightTypePoint;
  self->super.unit = unit;
  self->position = (CogVector3){0.0f, 0.0f, 0.0f};
  self->super.projector = projectedCubeMap != NULL ? True : False;
  self->projectedCubeMap = projectedCubeMap;
  
  self->constantAttenuation = 1.0f;
  self->linearAttenuation = 1.0f;
  self->quadraticAttenuation = 1.0f;
  
  self->range = 1.0f;
  
  self->shadowCubeMap = shadowCubeMap;
  
  self->super.free = _pointLightFree;
  self->super.inspect = _pointLightInspect;
  
  if(bundle) {
    CogBundleAddLight(bundle, (CogLight)self);
  }
  
  return self;
}




/////////////////////////
//  Directional Light  //
/////////////////////////
static void _directionalLightInspect(const CogLight self) {
  printf("<Directional Light '%s' @0x%ux>\n", self->name->data, (unsigned int)self);
}

static void _directionalLightFree(CogLight self) {
  CogDirectionalLight s = (CogDirectionalLight)self;
  CogStringFree(s->super.name);
  free(s);
}
CogDirectionalLight CogDirectionalLightAlloc(const char* name, struct CogScene* scene, struct CogNode* parent, const CogTexture projectedMap, CogTexture shadowMap, const int unit, struct CogBundle* bundle) {
  CogDirectionalLight self = malloc(sizeof(struct CogDirectionalLight));
  _setLight((CogLight)self, scene, parent, name);
  
  self->super.type = CogLightTypeDirectional;
  self->super.unit = unit;
  self->direction = (CogVector3){0.0f, 0.0f, 0.0f};
  self->super.projector = projectedMap != NULL ? True : False;
  self->projectedMap = projectedMap;
  self->shadowMap = shadowMap;
  
  self->super.free = _directionalLightFree;
  self->super.inspect = _directionalLightInspect;
  
  if(bundle) {
    CogBundleAddLight(bundle, (CogLight)self);
  }
  
  return self;
}




//////////////////
//  Spot Light  //
//////////////////
static void _spotLightInspect(const CogLight self) {
  printf("<Spot Light '%s' @0x%ux>\n", self->name->data, (unsigned int)self);
}

static void _spotLightFree(CogLight self) {
  CogSpotLight s = (CogSpotLight)self;
  CogStringFree(s->super.name);
  free(s);
}
CogSpotLight CogSpotLightAlloc(const char* name, struct CogScene* scene, struct CogNode* parent, const CogTexture projectedMap, CogTexture shadowMap, const int unit, struct CogBundle* bundle) {
  CogSpotLight self = malloc(sizeof(struct CogSpotLight));
  _setLight((CogLight)self, scene, parent, name);
  
  self->super.type = CogLightTypeSpot;
  self->super.unit = unit;
  self->position = (CogVector3){0.0f, 0.0f, 0.0f};
  self->direction = (CogVector3){0.0f, 0.0f, 0.0f};
  self->super.projector = projectedMap != NULL ? True : False;
  self->projectedMap = projectedMap;
  
  self->constantAttenuation = 1.0f;
  self->linearAttenuation = 1.0f;
  self->quadraticAttenuation = 1.0f;
  
  self->innerAngle = 45.0f;
  self->outerAngle = 90.0f;
  
  self->range = 1.0f;
  
  self->shadowMap = shadowMap;
  
  self->super.free = _spotLightFree;
  self->super.inspect = _spotLightInspect;
  
  if(bundle) {
    CogBundleAddLight(bundle, (CogLight)self);
  }
  
  return self;
}