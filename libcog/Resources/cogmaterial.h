#ifndef _COG_MATERIAL_H_
#define _COG_MATERIAL_H_

#include "cogstring.h"
#include "CogHash.h"
#include "cogtexture.h"
#include "cogcolor.h"

struct CogBundle;
struct CogShader;
struct CogMaterial {
  CogString              name;
  struct CogShader*      shader;
  
  struct {
    CogTexture          ambientOcclusionMap;
    float               ambientIntensity;
    float               ambientOcclusionStrenght;
    CogColor            ambientColor;
  } ambient;
  
  struct {
    CogTexture          diffuseMap;
    float               diffuseIntensity;
    CogColor            diffuseColor;
  } diffuse;
  
  struct {
    CogTexture          specularMap;
    float               specularIntensity;
    float               specularHardness; 
    CogColor            specularColor;
  } specular;
  
  struct {
    CogTexture          normalMap;
    CogTexture          heightMap;
    float               heightScale;
  } bump;

  struct {
    CogTexture          reflectionMap;
    float               reflectionStrenght;
    CogColor            reflectionTint;
  } reflection;
  
  struct {
    CogTexture          detailMap;
    float               detailStrenght;
  } detail;
  
  struct {
    CogTexture          glowMap;
    float               glowStrenght;
    CogColor            glowColor;
  } glow;

};
typedef struct CogMaterial* CogMaterial;
CogDefineHash(CogMaterial);

CogMaterial CogMaterialAlloc(const char* name, struct CogShader* shader, struct CogBundle* bundle);
void CogMaterialInspect(const CogMaterial self);
void CogMaterialFree(CogMaterial self);

#endif