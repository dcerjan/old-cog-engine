#include "cogrendererdeferred.h"
#include "cogresourcemanager.h"
#include "cogbundle.h"
#include "cogbundle.h"
#include "cogscenegraph.h"
#include "cogtexture.h"
#include "cogmesh.h"
#include "cogmodel.h"
#include "cognode.h"
#include "cogscene.h"
#include "cogmaterial.h"
#include "cogshader.h"
#include "cogframebuffer.h"
#include "cogsurface.h"
#include "cogrendertarget.h"
#include "cogcompositor.h"
#include "cogeffects.h"

static void _bindShader(CogRenderer self, const CogScene scene, const CogShader shader) {
  glUseProgram(shader->id);
}
static void _bindMaterial(CogRenderer self, const CogScene scene, const CogShader shader, const CogMaterial material) {
  // ambient
  glUniform1f(shader->uniforms.fragment.material.ambient.ambientIntensity, material->ambient.ambientIntensity);
  glUniform1f(shader->uniforms.fragment.material.ambient.ambientOcclusionStrenght, material->ambient.ambientOcclusionStrenght);
  glUniform4fv(shader->uniforms.fragment.material.ambient.ambientColor, 1, (float*)&material->ambient.ambientColor);
  if(material->ambient.ambientOcclusionMap) {
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(material->ambient.ambientOcclusionMap->type, material->ambient.ambientOcclusionMap->id);
    glUniform1i(shader->uniforms.fragment.material.ambient.ambientOcclusionMap, 4);
    glUniform1i(shader->uniforms.fragment.material.ambient.hasAmbientOcclusionMap, 1);
  } else {
    glUniform1i(shader->uniforms.fragment.material.ambient.hasAmbientOcclusionMap, 0);
  }
  
  // diffuse
  glUniform1f(shader->uniforms.fragment.material.diffuse.diffuseIntensity, material->diffuse.diffuseIntensity);
  glUniform4fv(shader->uniforms.fragment.material.diffuse.diffuseColor, 1, (float*)&material->diffuse.diffuseColor);
  if(material->diffuse.diffuseMap) { 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(material->diffuse.diffuseMap->type, material->diffuse.diffuseMap->id);
    glUniform1i(shader->uniforms.fragment.material.diffuse.diffuseMap, 0);
    glUniform1i(shader->uniforms.fragment.material.diffuse.hasDiffuseMap, 1);
  } else {
    glUniform1i(shader->uniforms.fragment.material.diffuse.hasDiffuseMap, 0);
  }
  
  // specular
  glUniform1f(shader->uniforms.fragment.material.specular.specularIntensity, material->specular.specularIntensity);
  glUniform1f(shader->uniforms.fragment.material.specular.specularHardness, material->specular.specularHardness);
  glUniform4fv(shader->uniforms.fragment.material.specular.specularColor, 1, (float*)&material->specular.specularColor);
  if(material->specular.specularMap) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(material->specular.specularMap->type, material->specular.specularMap->id);
    glUniform1i(shader->uniforms.fragment.material.specular.specularMap, 1);
    glUniform1i(shader->uniforms.fragment.material.specular.hasSpecularMap, 1);
  } else {
    glUniform1i(shader->uniforms.fragment.material.specular.hasSpecularMap, 0);
  }
  
  // bump
  glUniform1f(shader->uniforms.fragment.material.bump.heightScale, material->bump.heightScale);
  if(material->bump.normalMap) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(material->bump.normalMap->type, material->bump.normalMap->id);
    glUniform1i(shader->uniforms.fragment.material.bump.normalMap, 2);
    glUniform1i(shader->uniforms.fragment.material.bump.hasNormalMap, 1);
  } else {
    glUniform1i(shader->uniforms.fragment.material.bump.hasNormalMap, 0);
  }
  if(material->bump.heightMap) {
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(material->bump.heightMap->type, material->bump.heightMap->id);
    glUniform1i(shader->uniforms.fragment.material.bump.heightMap, 3);
    glUniform1i(shader->uniforms.fragment.material.bump.hasHeightMap, 1);
  } else {
    glUniform1i(shader->uniforms.fragment.material.bump.hasHeightMap, 0);
  }
  
  // detail
  glUniform1f(shader->uniforms.fragment.material.detail.detailStrenght, material->detail.detailStrenght);
  if(material->detail.detailMap) {
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(material->detail.detailMap->type, material->detail.detailMap->id);
    glUniform1i(shader->uniforms.fragment.material.detail.detailMap, 5);
    glUniform1i(shader->uniforms.fragment.material.detail.hasDetailMap, 1);
  } else {
    glUniform1i(shader->uniforms.fragment.material.detail.hasDetailMap, 0);
  }
  
  // reflection
  glUniform1f(shader->uniforms.fragment.material.reflection.reflectionStrenght, material->reflection.reflectionStrenght);
  glUniform4fv(shader->uniforms.fragment.material.reflection.reflectionTint, 1, (float*)&material->reflection.reflectionTint);
  if(material->reflection.reflectionMap) {
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(material->reflection.reflectionMap->type, material->reflection.reflectionMap->id);
    glUniform1i(shader->uniforms.fragment.material.reflection.reflectionMap, 6);
    glUniform1i(shader->uniforms.fragment.material.reflection.hasReflectionMap, 1);
  } else {
    glUniform1i(shader->uniforms.fragment.material.reflection.hasReflectionMap, 0);
  }
  
  // glow
  glUniform1f(shader->uniforms.fragment.material.glow.glowStrenght, material->glow.glowStrenght);
  glUniform4fv(shader->uniforms.fragment.material.glow.glowColor, 1, (float*)&material->glow.glowColor);
  if(material->glow.glowMap) {
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(material->glow.glowMap->type, material->glow.glowMap->id);
    glUniform1i(shader->uniforms.fragment.material.glow.glowMap, 7);
    glUniform1i(shader->uniforms.fragment.material.glow.hasGlowMap, 1);
  } else {
    glUniform1i(shader->uniforms.fragment.material.glow.hasGlowMap, 0);
  }
}

static void _bindCogMesh(CogRenderer self, const CogScene scene, const CogShader shader, const CogMesh mesh) {
  glBindVertexArray(mesh->id);
  glDrawArrays(mesh->drawMethod, 0, mesh->numElements);
  glBindVertexArray(0);
}

static void _bindCogNode(CogRenderer self, const CogScene scene, const CogShader shader, const CogNode node) {
  glUniformMatrix4fv(shader->uniforms.vertex.matrix.model, 1, GL_FALSE, (const float*)&node->model);
}

static void _bindLight(CogRenderer self, const CogScene scene, const CogShader shader, const CogLight light) {
  switch(light->type) {
    case CogLightTypeAmbient: {
        CogAmbientLight l = (CogAmbientLight)light;
        int i = l->super.unit;
        glUniform4f(shader->uniforms.fragment.light[i].color, l->super.color.r, l->super.color.g, l->super.color.b, l->super.color.a);
        glUniform1i(shader->uniforms.fragment.light[i].type, l->super.type);
      }
      break;
    case CogLightTypeDirectional: {
        CogDirectionalLight l = (CogDirectionalLight)light;
        int i = l->super.unit;
        glUniform4f(shader->uniforms.fragment.light[i].color, l->super.color.r, l->super.color.g, l->super.color.b, l->super.color.a);
        glUniform3f(shader->uniforms.fragment.light[i].direction, l->direction.x, l->direction.y, l->direction.z);
        glUniform1i(shader->uniforms.fragment.light[i].type, l->super.type);
      }
      break;
    case CogLightTypePoint: {
        CogPointLight l = (CogPointLight)light;
        int i = l->super.unit;
        glUniform4f(shader->uniforms.fragment.light[i].color, l->super.color.r, l->super.color.g, l->super.color.b, l->super.color.a);
        glUniform3f(shader->uniforms.fragment.light[i].position, l->position.x, l->position.y, l->position.z);
        glUniform1i(shader->uniforms.fragment.light[i].type, l->super.type);
        glUniform1f(shader->uniforms.fragment.light[i].constantAttenuation, l->constantAttenuation);
        glUniform1f(shader->uniforms.fragment.light[i].linearAttenuation, l->linearAttenuation);
        glUniform1f(shader->uniforms.fragment.light[i].quadraticAttenuation, l->quadraticAttenuation);
      }
      break;
    case CogLightTypeSpot: {
        CogSpotLight l = (CogSpotLight)light;
        int i = l->super.unit;
        glUniform4f(shader->uniforms.fragment.light[i].color, l->super.color.r, l->super.color.g, l->super.color.b, l->super.color.a);
        glUniform3f(shader->uniforms.fragment.light[i].position, l->position.x, l->position.y, l->position.z);
        glUniform3f(shader->uniforms.fragment.light[i].direction, l->direction.x, l->direction.y, l->direction.z);
        glUniform1i(shader->uniforms.fragment.light[i].type, l->super.type);
        glUniform1f(shader->uniforms.fragment.light[i].constantAttenuation, l->constantAttenuation);
        glUniform1f(shader->uniforms.fragment.light[i].linearAttenuation, l->linearAttenuation);
        glUniform1f(shader->uniforms.fragment.light[i].quadraticAttenuation, l->quadraticAttenuation);
        glUniform1f(shader->uniforms.fragment.light[i].innerAngle, l->innerAngle);
        glUniform1f(shader->uniforms.fragment.light[i].outerAngle, l->outerAngle);
      }
      break;
    default:
      return;
  }
}

static void _bindCogCamera(CogRenderer self, const CogScene scene, const CogShader shader, const CogCamera camera) {
  glUniformMatrix4fv(shader->uniforms.vertex.matrix.view, 1, GL_FALSE, (const float*)&camera->view);
  glUniformMatrix4fv(shader->uniforms.vertex.matrix.projection, 1, GL_FALSE, (const float*)&camera->projection);
  glUniform1f(shader->uniforms.vertex.camera.width, camera->width);
  glUniform1f(shader->uniforms.vertex.camera.height, camera->height);
  glUniform1f(shader->uniforms.vertex.camera.near, camera->near);
  glUniform1f(shader->uniforms.vertex.camera.far, camera->far);
  glUniform1f(shader->uniforms.vertex.camera.linearDepthConstant, 1.0f / (camera->far - camera->near));
}

static void _renderCogNode(CogRenderer self, const CogScene scene, const CogShader shader, const CogNode node) {
  CogListForEach(node->attachments.models, m, {
    _bindMaterial(self, scene, shader, m->material);
    _bindCogNode(self, scene, shader, node);
    _bindCogCamera(self, scene, shader, scene->activeCamera);
    _bindCogMesh(self, scene, shader, m->mesh);
  });
    
  CogListForEach(node->children, n, {
    _renderCogNode(self, scene, shader, n);
  });
}

static void _renderScene(CogRenderer self, const CogScene scene) {
  glClear(GL_COLOR_BUFFER_BIT);
  
  /*
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  */
  
  CogRendererDeferred r = (CogRendererDeferred)self;
  /* Preprocessing goes here */
  
  /*
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  */
  
  /* build deferred maps */
  self->bindShader(self, NULL, r->shaders.deferred.bake);
  CogFrameBufferBeginMultiple(
    r->fbo.deferredFbo, 
    4,
    (CogRenderTargetUnit[]){
      CogRenderTargetUnit0, 
      CogRenderTargetUnit1, 
      CogRenderTargetUnit2, 
      CogRenderTargetUnit3
    }
  );
  _renderCogNode(self, scene, r->shaders.deferred.bake, scene->sceneGraph->root);
  CogFrameBufferEnd();
  
  /*
  glDisable(GL_CULL_FACE);
  */
  
  /* Postprocessing goes here */
  
  /*
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
  */
  
  self->bindShader(self, NULL, r->shaders.deferred.compose);
  CogArrayForEach(scene->lights, l, { 
    _bindLight(self, scene, r->shaders.deferred.compose, l); 
  });
  CogSurfaceBlitMultipleToRenderTarget(
    r->surface, 4,
    (const struct CogTexture**)(CogTexture[]){
      r->rt.position->texture,
      r->rt.normal->texture,
      r->rt.diffuse->texture,
      r->rt.specular->texture
    },
    r->shaders.deferred.compose,
    r->fbo.compositionFbo,
    CogRenderTargetUnit0
  );
  
  CogTexture edgeDetect = NULL;
  CogTexture ssao = NULL;
  CogTexture ssr = NULL;
  CogTexture ssaa = NULL;
  CogTexture ssaaEdgeDetect = NULL;
  CogTexture bloom = NULL;
  
  if(r->videoOptions.edgeDetect.on) {
    if(r->videoOptions.edgeDetect.blur.on) {
      edgeDetect = CogEffectGaussianBlurRGBA(
        r->videoOptions.edgeDetect.blur.radius,
        r->videoOptions.edgeDetect.blur.numSamples,
        r->rt.normal->texture,
        r->fbo.screenSpaceFbo,
        CogRenderTargetUnit2,
        CogRenderTargetUnit3
      );
    }
    edgeDetect = CogEffectFreiChenDepth(
      r->videoOptions.edgeDetect.lowerBound, //0.9132f, 
      r->videoOptions.edgeDetect.upperBound, //0.9132f, 
      r->rt.normal->texture, 
      r->fbo.screenSpaceFbo, 
      CogRenderTargetUnit2
    );
  }
  
  
  if(r->videoOptions.ssao.on) {
     ssao = CogEffectSSAO(
      r->videoOptions.ssao.bias, //0.09f, 
      r->videoOptions.ssao.numSamples, //16, 
      r->videoOptions.ssao.radius, //32.0f, 
      r->videoOptions.ssao.constantAttenuation, //1.0f, 
      r->videoOptions.ssao.linearAttenuation, //1.0f, 
      r->videoOptions.ssao.quadraticAttenuation, //1.0f, 
      r->rt.position->texture, 
      r->rt.normal->texture, 
      r->fbo.screenSpaceFbo, 
      CogRenderTargetUnit0
    );
    
    if(r->videoOptions.ssao.blur.on) {
      ssao = CogEffectGaussianBlurR(
        r->videoOptions.ssao.blur.radius, 
        r->videoOptions.ssao.blur.numSamples,
        ssao, 
        r->fbo.screenSpaceFbo, 
        CogRenderTargetUnit1, 
        CogRenderTargetUnit3
      );
    }
  }
  
  /*
  if(r->videoOptions.ssaa.on) {
    ssaa = CogEffectGaussianBlurRGBA(
      r->videoOptions.ssaa.blur.radius,
      r->videoOptions.ssaa.blur.numSamples,
      r->fbo.compositionFbo->renderTargets[0]->texture,
      r->fbo.screenSpaceFbo,
      CogRenderTargetUnit1,
      CogRenderTargetUnit3
    );
  }
  */
  
  if(r->videoOptions.bloom.on) {
    bloom = CogEffectGaussianBlurRGBA(
      32.0f, 
      32, 
      r->fbo.compositionFbo->renderTargets[0]->texture, 
      r->fbo.bloomFbo, 
      CogRenderTargetUnit0, 
      CogRenderTargetUnit3 
    );
  }
  
  /*****************************/
  /** Effects and composition **/
  /*****************************/
  
  CogCompositorCompose/*ToRenderTarget*/(
    r->surface, 
    r->shaders.deferred.composeDeferred, 
    (const struct CogTexture**)(CogTexture[]){
      r->fbo.compositionFbo->renderTargets[0]->texture,
      ssao,
      edgeDetect,
      ssr,
      ssaa,
      ssaaEdgeDetect,
      bloom
    }, 7,
    (const float[]){}, 0,
    (const int[]){1,1,0,0,0,0, 0}, 7 /*,
    r->fbo.compositionFbo,
    CogRenderTargetUnit1*/
  );
  
  /*
  CogCompositorCompose(
    r->surface, 
    r->shaders.deferred.composeDeferred, 
    (const struct CogTexture**)(CogTexture[]){
      NULL, //r->fbo.compositionFbo->renderTargets[0]->texture,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      r->fbo.bloomFbo->renderTargets[0]->texture
    }, 7,
    (const float[]){}, 0,
    (const int[]){0,0,0,0,0,0,1}, 7
  );
  */
  
  /*******************/
  /** debug overlay **/
  /*******************/
  if(r->conf.debug.debugShaderOutput) {
    self->bindShader(self, NULL, r->debug.position);
    CogSurfaceBlit(r->debug.positionSurface, r->rt.position->texture, r->debug.position);
    self->bindShader(self, NULL, r->debug.depth);
    CogSurfaceBlit(r->debug.depthSurface, r->rt.position->texture, r->debug.depth);
    self->bindShader(self, NULL, r->debug.topology);
    CogSurfaceBlit(r->debug.topologySurface, r->rt.normal->texture, r->debug.topology);
    self->bindShader(self, NULL, r->debug.diffuse);
    CogSurfaceBlit(r->debug.diffuseSurface, r->rt.diffuse->texture, r->debug.diffuse);
    self->bindShader(self, NULL, r->debug.specular);
    CogSurfaceBlit(r->debug.specularSurface, r->rt.specular->texture, r->debug.specular);
  }
}

static void _cleanup(CogRenderer self) {
  
}

static void _inspect(CogRenderer self) {
  
}


CogRenderer CogRendererDeferredAlloc(CogRendererDeferredConfiguration conf) {
  CogRendererDeferred self = malloc(sizeof(struct CogRendererDeferred));
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(0.0f);
  
  glDepthFunc(GL_LEQUAL);
  
  /* Configure */
  self->conf.bloom.scale = conf.bloom.scale <= 0.1f || conf.bloom.scale > 1.0f ? 1.0f : conf.bloom.scale;
  self->conf.composition.scale = conf.composition.scale <= 0.1f || conf.composition.scale > 1.0f ? 1.0f : conf.composition.scale;
  self->conf.debug.debugShaderOutput = False ||conf.debug.debugShaderOutput;
  self->conf.deferred.scale = conf.deferred.scale <= 0.1f || conf.deferred.scale > 1.0f ? 1.0f : conf.deferred.scale;
  self->conf.deferred.useFP32PositionTarget = False || conf.deferred.useFP32PositionTarget;
  self->conf.lighting.maxLights = self->conf.lighting.maxLights <= 0 || self->conf.lighting.maxLights > 16 ? 8 : self->conf.lighting.maxLights;
  self->conf.resolution.height = conf.resolution.height > 240 ? conf.resolution.height : 720;
  self->conf.resolution.width = conf.resolution.width > 320 ? conf.resolution.width : 1280;
  self->conf.screenSpace.scale = conf.screenSpace.scale <= 0.1f || conf.screenSpace.scale > 1.0f ? 1.0f : conf.screenSpace.scale;
  self->conf.shadow.resolution = conf.shadow.resolution <= 256 || conf.shadow.resolution > 8096 ? 1024 : conf.shadow.resolution;
  
  /* Init functions */
  self->super.name = CogStringAlloc("Cog Deferred Renderer v0.1.0");
  self->super.free = _cleanup;
  self->super.inspect = _inspect;
  
  self->super.renderScene = (renderScene)_renderScene;
  self->super.renderCogNode = (renderCogNode)_renderCogNode;
  
  self->super.bindShader = (bindShader)_bindShader;
  self->super.bindCogCamera = (bindCogCamera)_bindCogCamera;
  self->super.bindCogNode = (bindCogNode)_bindCogNode;
  self->super.bindMaterial = (bindMaterial)_bindMaterial;
  self->super.bindCogMesh = (bindCogMesh)_bindCogMesh;
  self->super.bindLight = (bindLight)_bindLight;
  
  /* Assign builtin shaders */
  // for development only !! load required shaders
  CogResourceManagerLoadBundle("./builtins/bundles/default_deferred_renderer.json");
  self->store = CogResourceManagerGetBundle("Cog Deferred Renderer Bundle");
  self->debug.position = CogBundleGetShader(self->store, "Debug/Deferred/Position");
  self->debug.depth = CogBundleGetShader(self->store, "Debug/Deferred/Depth");
  self->debug.topology = CogBundleGetShader(self->store, "Debug/Deferred/Normal");
  self->debug.diffuse = CogBundleGetShader(self->store, "Debug/Deferred/Diffuse");
  self->debug.specular = CogBundleGetShader(self->store, "Debug/Deferred/Specular");
  
  self->shaders.deferred.bake = CogBundleGetShader(self->store, "Deferred/Bake");
  self->shaders.deferred.compose = CogBundleGetShader(self->store, "Deferred/Compose");
  self->shaders.deferred.composeDeferred = CogBundleGetShader(self->store, "Deferred/Compose/Deferred");
  
  self->shaders.util.blit = CogBundleGetShader(self->store, "Common/Blit");
  
  self->shaders.util.edgeDetect.freiChen.depth = CogBundleGetShader(self->store, "Util/FreiChen/Depth");
  self->shaders.util.edgeDetect.freiChen.depthNormal = CogBundleGetShader(self->store, "Util/FreiChen/DepthNormal");
  self->shaders.util.blur.gaussian.rgba = CogBundleGetShader(self->store, "Util/Blur/Gaussian/RGBA");
  self->shaders.util.blur.gaussian.rgb = CogBundleGetShader(self->store, "Util/Blur/Gaussian/RGB");
  self->shaders.util.blur.gaussian.rg = CogBundleGetShader(self->store, "Util/Blur/Gaussian/RG");
  self->shaders.util.blur.gaussian.r = CogBundleGetShader(self->store, "Util/Blur/Gaussian/R");
  self->shaders.util.composer = CogBundleGetShader(self->store, "Util/Composer");
  
  self->shaders.effect.ambientOcclusion = CogBundleGetShader(self->store, "Effect/SSAO");
  
  /* Allocate other renderer stuff */
  self->surface = CogSurfaceAlloc("Cog Default Deferred Renderer - Surface", (CogRect){.w = 1.0f, .h = 1.0f, .x = 0.0f, .y = 0.0f});
  self->debug.positionSurface = CogSurfaceAlloc("Cog Default Deferred Renderer - Debug Surface - position", (CogRect){.w = 0.2f, .h = 0.2f, .x = 0.0f, .y = 0.8f});
  self->debug.depthSurface = CogSurfaceAlloc("Cog Default Deferred Renderer - Debug Surface - depth", (CogRect){.w = 0.2f, .h = 0.2f, .x = 0.2f, .y = 0.8f});
  self->debug.topologySurface = CogSurfaceAlloc("Cog Default Deferred Renderer - Debug Surface - topology", (CogRect){.w = 0.2f, .h = 0.2f, .x = 0.4f, .y = 0.8f});
  self->debug.diffuseSurface = CogSurfaceAlloc("Cog Default Deferred Renderer - Debug Surface - diffuse", (CogRect){.w = 0.2f, .h = 0.2f, .x = 0.6f, .y = 0.8f});
  self->debug.specularSurface = CogSurfaceAlloc("Cog Default Deferred Renderer - Debug Surface - specular", (CogRect){.w = 0.2f, .h = 0.2f, .x = 0.8f, .y = 0.8f});
  
  
  // Textures
  self->randommap = CogTextureAllocFromFile("./builtinsimages/randomnormalmap.png", "Random Normal Map", self->store);
  
  // Deferred FBO
  self->fbo.deferredFbo = CogFrameBufferAlloc(
    "Cog Deferred Renderer - Deferred FBO", 
    self->conf.resolution.width * self->conf.deferred.scale, 
    self->conf.resolution.height * self->conf.deferred.scale
  );
  CogFrameBufferEnableDepthBuffer(self->fbo.deferredFbo, 1.0f);
  CogFrameBufferAttachRenderTarget(self->fbo.deferredFbo, "Cog Default Deferred Renderer - RT - Position", CogColorFormatRGBA, self->conf.deferred.useFP32PositionTarget ? CogColorInternalFormatRGBA32F : CogColorInternalFormatRGBA16F, CogRenderTargetUnit0);
  CogFrameBufferAttachRenderTarget(self->fbo.deferredFbo, "Cog Default Deferred Renderer - RT - Topology", CogColorFormatRGBA, CogColorInternalFormatRGBA8, CogRenderTargetUnit1);
  CogFrameBufferAttachRenderTarget(self->fbo.deferredFbo, "Cog Default Deferred Renderer - RT - Diffuse", CogColorFormatRGBA, CogColorInternalFormatRGBA8, CogRenderTargetUnit2);
  CogFrameBufferAttachRenderTarget(self->fbo.deferredFbo, "Cog Default Deferred Renderer - RT - Specular", CogColorFormatRGBA, CogColorInternalFormatRGBA8, CogRenderTargetUnit3);
  CogFrameBufferCheck(self->fbo.deferredFbo);
  self->rt.position = self->fbo.deferredFbo->renderTargets[0];
  self->rt.normal = self->fbo.deferredFbo->renderTargets[1];
  self->rt.diffuse = self->fbo.deferredFbo->renderTargets[2];
  self->rt.specular = self->fbo.deferredFbo->renderTargets[3];
  
  // Bloom FBO
  self->fbo.bloomFbo = CogFrameBufferAlloc(
    "Cog Deferred Renderer - Bloom FBO", 
    self->conf.resolution.width * self->conf.bloom.scale, 
    self->conf.resolution.height * self->conf.bloom.scale
  );
  CogFrameBufferEnableDepthBuffer(self->fbo.bloomFbo, 1.0f);
  CogFrameBufferAttachRenderTarget(self->fbo.bloomFbo, "Cog Deferred Renderer - Bloom RT", CogColorFormatRGB, CogColorInternalFormatRGB8, CogRenderTargetUnit0);
  CogFrameBufferAttachRenderTarget(self->fbo.bloomFbo, "Cog Deferred Renderer - Bloom Aux 1 RT", CogColorFormatRGB, CogColorInternalFormatRGB8, CogRenderTargetUnit1);
  CogFrameBufferAttachRenderTarget(self->fbo.bloomFbo, "Cog Deferred Renderer - Bloom Aux 2 RT", CogColorFormatRGB, CogColorInternalFormatRGB8, CogRenderTargetUnit2);
  CogFrameBufferAttachRenderTarget(self->fbo.bloomFbo, "Cog Deferred Renderer - Bloom Ping Pong RT", CogColorFormatRGB, CogColorInternalFormatRGB8, CogRenderTargetUnit3);
  CogFrameBufferCheck(self->fbo.bloomFbo);
  
  // Shadow FBO
  self->fbo.shadowMapFbo = CogFrameBufferAlloc(
    "Cog Deferred Renderer - Shadow FBO", 
    self->conf.shadow.resolution, 
    self->conf.shadow.resolution
  );
  CogFrameBufferEnableDepthBuffer(self->fbo.shadowMapFbo, 1.0f);
  CogFrameBufferAttachRenderTarget(self->fbo.shadowMapFbo, "Cog Deferred Renderer - Shadow RT", CogColorFormatRGBA, CogColorInternalFormatRGBA8, CogRenderTargetUnit0);
  CogFrameBufferAttachRenderTarget(self->fbo.shadowMapFbo, "Cog Deferred Renderer - Shadow Ping Pong RT", CogColorFormatRGBA, CogColorInternalFormatRGBA8, CogRenderTargetUnit1);
  CogFrameBufferCheck(self->fbo.shadowMapFbo);
  
  // Compositor FBO
  self->fbo.compositionFbo = CogFrameBufferAlloc(
    "Cog Deferred Renderer - Compositor FBO", 
    self->conf.resolution.width * self->conf.composition.scale, 
    self->conf.resolution.height * self->conf.composition.scale
  );
  CogFrameBufferEnableDepthBuffer(self->fbo.compositionFbo, 1.0f);
  CogFrameBufferAttachRenderTarget(self->fbo.compositionFbo, "Cog Deferred Renderer - Composition RT", CogColorFormatRGB, CogColorInternalFormatRGB8, CogRenderTargetUnit0);
  CogFrameBufferAttachRenderTarget(self->fbo.compositionFbo, "Cog Deferred Renderer - Composition Aux 1 RT", CogColorFormatRGB, CogColorInternalFormatRGB8, CogRenderTargetUnit1);
  CogFrameBufferAttachRenderTarget(self->fbo.compositionFbo, "Cog Deferred Renderer - Composition Aux 2 RT", CogColorFormatRGB, CogColorInternalFormatRGB8, CogRenderTargetUnit2);
  CogFrameBufferAttachRenderTarget(self->fbo.compositionFbo, "Cog Deferred Renderer - Composition Ping Pong RT", CogColorFormatRGB, CogColorInternalFormatRGB8, CogRenderTargetUnit3);
  CogFrameBufferCheck(self->fbo.compositionFbo);
  
  // ScreenSpace FBO
  self->fbo.screenSpaceFbo = CogFrameBufferAlloc(
    "Cog Deferred Renderer - ScreenSpace FBO", 
    self->conf.resolution.width * self->conf.screenSpace.scale, 
    self->conf.resolution.height * self->conf.screenSpace.scale
  );
  CogFrameBufferEnableDepthBuffer(self->fbo.screenSpaceFbo, 1.0f);
  CogFrameBufferAttachRenderTarget(self->fbo.screenSpaceFbo, "Cog Deferred Renderer - SSAO RT", CogColorFormatRED, CogColorInternalFormatR8, CogRenderTargetUnit0);
  CogFrameBufferAttachRenderTarget(self->fbo.screenSpaceFbo, "Cog Deferred Renderer - SSR RT", CogColorFormatRGBA, CogColorInternalFormatRGBA8, CogRenderTargetUnit1);
  CogFrameBufferAttachRenderTarget(self->fbo.screenSpaceFbo, "Cog Deferred Renderer - EdgeDetect RT", CogColorFormatRED, CogColorInternalFormatR8, CogRenderTargetUnit2);
  CogFrameBufferAttachRenderTarget(self->fbo.screenSpaceFbo, "Cog Deferred Renderer - Screen Space Ping Pong RT", CogColorFormatRGBA, CogColorInternalFormatRGBA8, CogRenderTargetUnit3);
  CogFrameBufferCheck(self->fbo.screenSpaceFbo);
  
  return (CogRenderer)self;
}

