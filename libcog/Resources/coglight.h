#ifndef _COG_LIGHT_H_
#define _COG_LIGHT_H_

#include "cogtypes.h"
#include "cogstring.h"
#include "CogHash.h"
#include "cogcolor.h"
#include "CogVector3.h"
#include "cogtexture.h"

struct CogBundle;
struct CogShader;
struct CogNode;
struct CogScene;
typedef enum CogLightType {
  CogLightTypeUndefined = 0,
  CogLightTypeAmbient = 1,
  CogLightTypePoint = 2,
  CogLightTypeDirectional = 3,
  CogLightTypeSpot = 4
} CogLightType;

typedef enum CogShadowType {
  CogShadowTypeNone = 0,
  CogShadowTypeHard = 1,
  CogShadowTypeSoft = 2
} CogShadowType;

//////////////////////////
//  Base Light  Object  //
//////////////////////////
  struct CogLight {
    CogString       name;
    CogLightType    type;
    CogColor        color;
    CogShadowType   shadow;
    int             shadowCascades;
    Bool            projector;
    float           intensity;
    int             unit;
    struct CogNode*    parent;
    
    void (*free)(struct CogLight*);
    void (*inspect)(struct CogLight*);
  };
  typedef struct CogLight* CogLight;
  CogDefineArray(CogLight);
  CogDefineList(CogLight);
  CogDefineHash(CogLight);

  void CogLightInspect(const CogLight self);
  void CogLightFree(CogLight self);


/////////////////////
//  Ambient Light  //
/////////////////////
  struct CogAmbientLight {
    CogInherit(struct CogLight);
  };
  typedef struct CogAmbientLight* CogAmbientLight;
  CogAmbientLight  CogAmbientLightAlloc(const char* name, struct CogScene* scene, struct CogNode* parent, const int unit, struct CogBundle* bundle);
  

///////////////////
//  Point Light  //
///////////////////
  struct CogPointLight {
    CogInherit(struct CogLight);
    
    CogVector3     position;
    
    CogTexture     projectedCubeMap;
    CogTexture     shadowCubeMap;
    
    float       constantAttenuation;
    float       linearAttenuation;
    float       quadraticAttenuation;
    float       range;
  };
  typedef struct CogPointLight* CogPointLight;
  CogPointLight  CogPointLightAlloc(const char* name, struct CogScene* scene, struct CogNode* parent, CogTexture projectedCubeMap, CogTexture shadowCubeMap, const int unit, struct CogBundle* bundle);


/////////////////////////
//  Directional Light  //
/////////////////////////
  struct CogDirectionalLight {
    CogInherit(struct CogLight);
    
    CogVector3   direction;
    
    CogTexture   projectedMap;
    CogTexture   shadowMap;
  };
  typedef struct CogDirectionalLight* CogDirectionalLight;
  CogDirectionalLight CogDirectionalLightAlloc(const char* name, struct CogScene* scene, struct CogNode* parent, CogTexture projectedMap, CogTexture shadowMap, const int unit, struct CogBundle* bundle);


//////////////////
//  Spot Light  //
//////////////////
  struct CogSpotLight {
    CogInherit(struct CogLight);
    
    CogVector3     direction;
    CogVector3     position;
    
    CogTexture     projectedMap;
    CogTexture     shadowMap;
    
    float       innerAngle;
    float       outerAngle;
    float       constantAttenuation;
    float       linearAttenuation;
    float       quadraticAttenuation;
    float       range;
  };
  typedef struct CogSpotLight* CogSpotLight;
  CogSpotLight CogSpotLightAlloc(const char* name, struct CogScene* scene, struct CogNode* parent, CogTexture projectedMap, CogTexture shadowMap, const int unit, struct CogBundle* bundle);

#endif