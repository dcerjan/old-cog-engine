#ifndef _COG_SHADER_H_
#define _COG_SHADER_H_

#include "cogtypes.h"
#include "cogstring.h"
#include "coggl.h"
#include "cogmaterial.h"
#include "coglight.h"

struct CogBundle;
struct CogShader {
  CogString        name;
  GLuint        id;
  
  struct {
    struct {
      struct {
        GLuint  model;
        GLuint  view;
        GLuint  projection;
      } matrix;
      struct {
        GLuint  width;
        GLuint  height;
        GLuint  near;
        GLuint  far;
        GLuint  linearDepthConstant;
      } camera;
    } vertex;
    
    struct {
      
    } geometry;
    
    struct {
      struct {
        struct {
          GLuint  ambientOcclusionMap;
          GLuint  ambientOcclusionStrenght;
          GLuint  ambientIntensity;
          GLuint  ambientColor;
          GLuint  hasAmbientOcclusionMap;
        } ambient;
        struct {
          GLuint  diffuseMap;
          GLuint  diffuseIntensity;
          GLuint  diffuseColor;
          GLuint  hasDiffuseMap;
        } diffuse;
        struct {
          GLuint  specularMap;
          GLuint  specularIntensity;
          GLuint  specularHardness; 
          GLuint  specularColor;
          GLuint  hasSpecularMap;
        } specular;
        struct {
          GLuint  normalMap;
          GLuint  heightMap;
          GLuint  heightScale;
          GLuint  hasNormalMap;
          GLuint  hasHeightMap;
        } bump;
        struct {
          GLuint  reflectionMap;
          GLuint  reflectionStrenght;
          GLuint  reflectionTint;
          GLuint  hasReflectionMap;
        } reflection;
        struct {
          GLuint  detailMap;
          GLuint  detailStrenght;
          GLuint  hasDetailMap;
        } detail;
        struct {
          GLuint  glowMap;
          GLuint  glowStrenght;
          GLuint  glowColor;
          GLuint  hasGlowMap;
        } glow;
      } material;
      struct {
        GLuint  type;
        GLuint  position;
        GLuint  direction;
        GLuint  constantAttenuation;
        GLuint  linearAttenuation;
        GLuint  quadraticAttenuation;
        GLuint  color;
        GLuint  innerAngle;
        GLuint  outerAngle;
      } light[16];
      struct {
        GLuint  texChannel[16];
        GLuint  floatChannel[16];
        GLuint  intChannel[16];
      } compositor;
    } fragment;
  } uniforms;
};
typedef struct CogShader* CogShader;
CogDefineArray(CogShader);
CogDefineList(CogShader);
CogDefineHash(CogShader);

CogShader CogShaderAlloc(const char* name, const char** vertexSource, const char** geometrySource, const char** fragmentSource, struct CogBundle* bundle);
void CogShaderInspect(const CogShader self);
void CogShaderFree(CogShader self);

#endif