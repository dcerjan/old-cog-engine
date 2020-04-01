#include "cogshader.h"
#include "coglogger.h"
#include "cogplatform.h"
#include "cogbundle.h"

CogShader CogShaderAlloc(const char* name, const char** vertexSource, const char** geometrySource, const char** fragmentSource, struct CogBundle* bundle) {
  CogShader self = malloc(sizeof(struct CogShader));
  int i;
  
  self->name = CogStringAlloc(name);
  // clear uniform locations
  // fragment
  self->uniforms.fragment.material.ambient.ambientColor = -1;
  self->uniforms.fragment.material.ambient.ambientIntensity = -1;
  self->uniforms.fragment.material.ambient.ambientOcclusionMap = -1;
  self->uniforms.fragment.material.ambient.ambientOcclusionStrenght = -1;
  
  self->uniforms.fragment.material.diffuse.diffuseColor = -1;
  self->uniforms.fragment.material.diffuse.diffuseIntensity = -1;
  self->uniforms.fragment.material.diffuse.diffuseMap = -1;
  
  self->uniforms.fragment.material.specular.specularColor = -1;
  self->uniforms.fragment.material.specular.specularHardness = -1;
  self->uniforms.fragment.material.specular.specularIntensity = -1;
  self->uniforms.fragment.material.specular.specularMap = -1;
  
  self->uniforms.fragment.material.bump.heightMap = -1;
  self->uniforms.fragment.material.bump.heightScale = -1;
  self->uniforms.fragment.material.bump.normalMap = -1;
  
  self->uniforms.fragment.material.reflection.reflectionMap = -1;
  self->uniforms.fragment.material.reflection.reflectionStrenght = -1;
  self->uniforms.fragment.material.reflection.reflectionTint = -1;
  
  self->uniforms.fragment.material.detail.detailMap = -1;
  self->uniforms.fragment.material.detail.detailStrenght = -1;
  
  self->uniforms.fragment.material.glow.glowColor = -1;
  self->uniforms.fragment.material.glow.glowMap = -1;
  self->uniforms.fragment.material.glow.glowStrenght = -1;
  
  for(i = 0; i < 16; ++i) {
    self->uniforms.fragment.light[i].color = -1;
    self->uniforms.fragment.light[i].constantAttenuation = -1;
    self->uniforms.fragment.light[i].direction = -1;
    self->uniforms.fragment.light[i].innerAngle = -1;
    self->uniforms.fragment.light[i].linearAttenuation = -1;
    self->uniforms.fragment.light[i].outerAngle = -1;
    self->uniforms.fragment.light[i].position = -1;
    self->uniforms.fragment.light[i].quadraticAttenuation = -1;
    self->uniforms.fragment.light[i].type = -1;
  }
  
  for(i = 0; i < 16; ++i) {
    self->uniforms.fragment.compositor.texChannel[i] = -1;
    self->uniforms.fragment.compositor.floatChannel[i] = -1;
    self->uniforms.fragment.compositor.intChannel[i] = -1;
  }
  // geometry
  // vertex
  self->uniforms.vertex.matrix.model = -1;
  self->uniforms.vertex.matrix.view = -1;
  self->uniforms.vertex.matrix.projection = -1;

  self->uniforms.vertex.camera.far = -1;
  self->uniforms.vertex.camera.height = -1;
  self->uniforms.vertex.camera.linearDepthConstant = -1;
  self->uniforms.vertex.camera.near = -1;
  self->uniforms.vertex.camera.width = -1;
  
  self->id = glCreateProgram();
  
  GLuint vs, gs, fs;
  int vslen, gslen, fslen;
  
  if(!vertexSource || !fragmentSource) {
    CogLoggerError(CogPlatformGetLogger(), "CogVertex and fragment shader stage can not be ommited for shader <%s>.\n", name);
  } else {
    
    vslen = (int)strlen(*vertexSource);
    vs = glCreateShader(GL_VERTEX_SHADER);
    
    if(geometrySource) {
      gslen = (int)strlen(*geometrySource);
      gs = glCreateShader(GL_GEOMETRY_SHADER);
    }
    
    fslen = (int)strlen(*fragmentSource);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    
    // compile vertex shader
    glShaderSource(vs, 1, vertexSource, &vslen);
    glCompileShader(vs);
    
    GLint result;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        char *gllog;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &vslen);
        gllog = malloc(vslen);
        glGetShaderInfoLog(vs, vslen, &result, gllog);
        CogLoggerError(CogPlatformGetLogger(), "Unable to compile vertex shader source from CogString for shader <%s>, reason:\n%s\n", self->name, gllog);
        free(gllog);
        glDeleteShader(vs);
        exit(-1);
    }
    
    if(geometrySource) {
      // compile vertex shader
      glShaderSource(gs, 1, geometrySource, &gslen);
      glCompileShader(gs);
      
      glGetShaderiv(gs, GL_COMPILE_STATUS, &result);
      if(result == GL_FALSE) {
          char *gllog;
          glGetShaderiv(gs, GL_INFO_LOG_LENGTH, &gslen);
          gllog = malloc(gslen);
          glGetShaderInfoLog(gs, gslen, &result, gllog);
          CogLoggerError(CogPlatformGetLogger(), "Unable to compile geometry shader source from CogString for shader <%s>, reason:\n%s\n", self->name, gllog);
          free(gllog);
          glDeleteShader(gs);
          exit(-1);
      }
    }
    
    // compile vertex shader
    glShaderSource(fs, 1,  fragmentSource, &fslen);
    glCompileShader(fs);
    
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        char *gllog;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &fslen);
        gllog = malloc(fslen);
        glGetShaderInfoLog(fs, fslen, &result, gllog);
        CogLoggerError(CogPlatformGetLogger(), "Unable to compile fragment shader source from CogString for shader <%s>, reason:\n%s\n", self->name, gllog);
        free(gllog);
        glDeleteShader(fs);
        exit(-1);
    }
    
    glAttachShader(self->id, vs);
    if(geometrySource) { glAttachShader(self->id, gs); }
    glAttachShader(self->id, fs);
    
    glLinkProgram(self->id);
    glGetProgramiv(self->id, GL_LINK_STATUS, &result);
    if(result == GL_FALSE) {
      GLint len;
      char *gllog;
      
      glGetProgramiv(self->id, GL_INFO_LOG_LENGTH, &len);
      gllog = malloc(len);
      glGetProgramInfoLog(self->id, len, &result, gllog);
      CogLoggerError(CogPlatformGetLogger(), "Unable to link program '%s'.\n reason: \n%s\n", name, gllog);
      free(gllog);
      
      glDeleteProgram(self->id);
      exit(-1);
    }
    
    glDeleteShader(vs);
    if(geometrySource) { glDeleteShader(gs); }
    glDeleteShader(fs);
    /* precache uniform locations */
    self->uniforms.vertex.matrix.model = glGetUniformLocation(self->id, "model");
    self->uniforms.vertex.matrix.view = glGetUniformLocation(self->id, "view");
    self->uniforms.vertex.matrix.projection = glGetUniformLocation(self->id, "projection");
    
    self->uniforms.vertex.camera.far = glGetUniformLocation(self->id, "far");
    self->uniforms.vertex.camera.near = glGetUniformLocation(self->id, "near");
    self->uniforms.vertex.camera.width = glGetUniformLocation(self->id, "width");
    self->uniforms.vertex.camera.height = glGetUniformLocation(self->id, "height");
    self->uniforms.vertex.camera.linearDepthConstant = glGetUniformLocation(self->id, "linearDepthConstant");
    
    self->uniforms.fragment.material.ambient.ambientColor = glGetUniformLocation(self->id, "ambientColor");
    self->uniforms.fragment.material.ambient.ambientIntensity = glGetUniformLocation(self->id, "ambientIntensity");
    self->uniforms.fragment.material.ambient.ambientOcclusionMap = glGetUniformLocation(self->id, "ambientOcclusionMap");
    self->uniforms.fragment.material.ambient.ambientOcclusionStrenght = glGetUniformLocation(self->id, "ambientOcclusionStrength");
    self->uniforms.fragment.material.ambient.hasAmbientOcclusionMap = glGetUniformLocation(self->id, "hasAmbientOcclusionMap");
    
    self->uniforms.fragment.material.diffuse.diffuseColor = glGetUniformLocation(self->id, "diffuseColor");
    self->uniforms.fragment.material.diffuse.diffuseIntensity = glGetUniformLocation(self->id, "diffuseIntensity");
    self->uniforms.fragment.material.diffuse.diffuseMap = glGetUniformLocation(self->id, "diffuseMap");
    self->uniforms.fragment.material.diffuse.hasDiffuseMap = glGetUniformLocation(self->id, "hasDiffuseMap");
    
    self->uniforms.fragment.material.specular.specularColor = glGetUniformLocation(self->id, "specularColor");
    self->uniforms.fragment.material.specular.specularHardness = glGetUniformLocation(self->id, "specularHardness");
    self->uniforms.fragment.material.specular.specularIntensity = glGetUniformLocation(self->id, "specularIntensity");
    self->uniforms.fragment.material.specular.specularMap = glGetUniformLocation(self->id, "specularMap");
    self->uniforms.fragment.material.specular.hasSpecularMap = glGetUniformLocation(self->id, "hasSpecularMap");
    
    self->uniforms.fragment.material.bump.heightMap = glGetUniformLocation(self->id, "heightMap");
    self->uniforms.fragment.material.bump.heightScale = glGetUniformLocation(self->id, "heightScale");
    self->uniforms.fragment.material.bump.normalMap = glGetUniformLocation(self->id, "normalMap");
    self->uniforms.fragment.material.bump.hasNormalMap = glGetUniformLocation(self->id, "hasNormalMap");
    self->uniforms.fragment.material.bump.hasHeightMap = glGetUniformLocation(self->id, "hasHeightMap");
    
    self->uniforms.fragment.material.reflection.reflectionMap = glGetUniformLocation(self->id, "reflectionMap");
    self->uniforms.fragment.material.reflection.reflectionStrenght = glGetUniformLocation(self->id, "reflectionStrenght");
    self->uniforms.fragment.material.reflection.reflectionTint = glGetUniformLocation(self->id, "reflectionTint");
    self->uniforms.fragment.material.reflection.hasReflectionMap = glGetUniformLocation(self->id, "hasReflectionMap");
    
    self->uniforms.fragment.material.detail.detailMap = glGetUniformLocation(self->id, "detailMap");
    self->uniforms.fragment.material.detail.detailStrenght = glGetUniformLocation(self->id, "detailStrenght");
    self->uniforms.fragment.material.detail.hasDetailMap = glGetUniformLocation(self->id, "hasDetailMap");
    
    self->uniforms.fragment.material.glow.glowColor = glGetUniformLocation(self->id, "glowColor");
    self->uniforms.fragment.material.glow.glowMap = glGetUniformLocation(self->id, "glowMap");
    self->uniforms.fragment.material.glow.glowStrenght = glGetUniformLocation(self->id, "glowStrenght");
    self->uniforms.fragment.material.glow.hasGlowMap = glGetUniformLocation(self->id, "hasGlowMap");
    
    char buffer[512];
    for(i = 0; i < 16; ++i) {
      sprintf(buffer, "lightType[%d]", i); 
      self->uniforms.fragment.light[i].type = glGetUniformLocation(self->id, buffer);
      
      sprintf(buffer, "lightColor[%d]", i); 
      self->uniforms.fragment.light[i].color = glGetUniformLocation(self->id, buffer);
      
      sprintf(buffer, "lightDirection[%d]", i); 
      
      self->uniforms.fragment.light[i].direction = glGetUniformLocation(self->id, buffer);
      
      sprintf(buffer, "lightPosition[%d]", i); 
      self->uniforms.fragment.light[i].position = glGetUniformLocation(self->id, buffer);
      
      sprintf(buffer, "lightInnerAngle[%d]", i); 
      self->uniforms.fragment.light[i].innerAngle = glGetUniformLocation(self->id, buffer);
      
      sprintf(buffer, "lightOuterAngle[%d]", i); 
      self->uniforms.fragment.light[i].outerAngle = glGetUniformLocation(self->id, buffer);
      
      sprintf(buffer, "lightConstantAttenuation[%d]", i); 
      self->uniforms.fragment.light[i].constantAttenuation = glGetUniformLocation(self->id, buffer);
      
      sprintf(buffer, "lightLinearAttenuation[%d]", i); 
      self->uniforms.fragment.light[i].linearAttenuation = glGetUniformLocation(self->id, buffer);
      
      sprintf(buffer, "lightQuadraticAttenuation[%d]", i); 
      self->uniforms.fragment.light[i].quadraticAttenuation = glGetUniformLocation(self->id, buffer);
    }
    
    for(i = 0; i < 16; ++i) {
      sprintf(buffer, "texChannel[%d]", i);
      self->uniforms.fragment.compositor.texChannel[i] = glGetUniformLocation(self->id, buffer);
      sprintf(buffer, "floatChannel[%d]", i);
      self->uniforms.fragment.compositor.floatChannel[i] = glGetUniformLocation(self->id, buffer);
      sprintf(buffer, "intChannel[%d]", i);
      self->uniforms.fragment.compositor.intChannel[i] = glGetUniformLocation(self->id, buffer);
    }
    
    CogLoggerInfo(CogPlatformGetLogger(), "[Resource] Shader <%s> created.\n", self->name->data);
  }
  
  if(bundle) {
    CogBundleAddShader(bundle, self);
  }
  
  return self;
}

void CogShaderInspect(const CogShader self) {
  
}

void CogShaderFree(CogShader self) {
  CogLoggerInfo(CogPlatformGetLogger(), "[Resource] Shader <%s> deleted.\n", self->name->data);
  glDeleteProgram(self->id);
  CogStringFree(self->name);
  free(self);
}