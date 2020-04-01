#include "cogbundle.h"
#include "cogplatform.h"

CogBundle CogBundleAlloc(const char* name) {
  CogBundle self = malloc(sizeof(struct CogBundle));
  self->name = CogStringAlloc(name);
  self->pool = CogCleanupPoolAlloc(name);
  
  self->resources.audioSources = CogHashAlloc(CogAudioSource);
  self->resources.fonts = CogHashAlloc(CogFont);
  self->resources.images = CogHashAlloc(CogImage);
  self->resources.lights = CogHashAlloc(CogLight);
  self->resources.materials = CogHashAlloc(CogMaterial);
  self->resources.meshes = CogHashAlloc(CogMesh);
  self->resources.rawMeshes = CogHashAlloc(CogRawMesh);
  self->resources.models = CogHashAlloc(CogModel);
  self->resources.shaders = CogHashAlloc(CogShader);
  self->resources.skeletons = CogHashAlloc(CogSkeleton);
  self->resources.textures = CogHashAlloc(CogTexture);
  self->resources.cameras = CogHashAlloc(CogCamera);
  
  CogLoggerInfo(CogPlatformGetLogger(), "Bundle <%s> created.\n", name);
  return self;
}

void CogBundleInspect(const CogBundle self) {
  
}

void CogBundleFree(CogBundle self) {
  CogString tmp = self->name;
  CogLoggerInfo(CogPlatformGetLogger(), "Bundle <%s> cleanup:\n", tmp->data);
  
  CogCleanupPoolFree(self->pool);
  CogHashFree(self->resources.audioSources);
  CogHashFree(self->resources.cameras);
  CogHashFree(self->resources.fonts);
  CogHashFree(self->resources.images);
  CogHashFree(self->resources.lights);
  CogHashFree(self->resources.materials);
  CogHashFree(self->resources.meshes);
  CogHashFree(self->resources.rawMeshes);
  CogHashFree(self->resources.models);
  CogHashFree(self->resources.shaders);
  CogHashFree(self->resources.skeletons);
  CogHashFree(self->resources.textures);
  free(self);
  
  CogLoggerInfo(CogPlatformGetLogger(), "Bundle <%s> deleted.\n", tmp->data);
  CogStringFree(tmp);
}

void CogBundleAddCogMesh(CogBundle self, const CogMesh mesh) {
  CogCleanupPoolRegisterForCleanup(self->pool, mesh, (auto_destruct)CogMeshFree);
  CogHashSet(self->resources.meshes, mesh->name->data, mesh);
}

void CogBundleAddCogRawMesh(CogBundle self, const CogRawMesh rawCogMesh) {
  CogCleanupPoolRegisterForCleanup(self->pool, rawCogMesh, (auto_destruct)CogRawMeshFree);
  CogHashSet(self->resources.rawMeshes, rawCogMesh->name->data, rawCogMesh);
}

void CogBundleAddShader(CogBundle self, const CogShader shader) {
  CogCleanupPoolRegisterForCleanup(self->pool, shader, (auto_destruct)CogShaderFree);
  CogHashSet(self->resources.shaders, shader->name->data, shader);
}

void CogBundleAddLight(CogBundle self, const CogLight light) {
  CogCleanupPoolRegisterForCleanup(self->pool, light, (auto_destruct)CogLightFree);
  CogHashSet(self->resources.lights, light->name->data, light);
}

void CogBundleAddCogCamera(CogBundle self, const CogCamera CogCamera) {
  CogCleanupPoolRegisterForCleanup(self->pool, CogCamera, (auto_destruct)CogCameraFree);
  CogHashSet(self->resources.cameras, CogCamera->name->data, CogCamera);
}

void CogBundleAddSkeleton(CogBundle self, const CogSkeleton skeleton) {
  CogCleanupPoolRegisterForCleanup(self->pool, skeleton, (auto_destruct)CogSkeletonFree);
  CogHashSet(self->resources.skeletons, skeleton->name->data, skeleton);
}

void CogBundleAddImage(CogBundle self, const CogImage image) {
  CogCleanupPoolRegisterForCleanup(self->pool, image, (auto_destruct)CogImageFree);
  CogHashSet(self->resources.images, image->name->data, image);
}

void CogBundleAddFont(CogBundle self, const CogFont font) {
  CogCleanupPoolRegisterForCleanup(self->pool, font, (auto_destruct)CogFontFree);
  CogHashSet(self->resources.fonts, font->name->data, font);
}

void CogBundleAddAudioSource(CogBundle self, const CogAudioSource audioSource) {
  CogCleanupPoolRegisterForCleanup(self->pool, audioSource, (auto_destruct)CogAudioSourceFree);
  CogHashSet(self->resources.audioSources, audioSource->name->data, audioSource);
}

void CogBundleAddTexture(CogBundle self, const CogTexture texture) {
  CogCleanupPoolRegisterForCleanup(self->pool, texture, (auto_destruct)CogTextureFree);
  CogHashSet(self->resources.textures, texture->name->data, texture);
}

void CogBundleAddMaterial(CogBundle self, const CogMaterial material) {
  CogCleanupPoolRegisterForCleanup(self->pool, material, (auto_destruct)CogMaterialFree);
  CogHashSet(self->resources.materials, material->name->data, material);
}

void CogBundleAddModel(CogBundle self, const CogModel model) {
  CogCleanupPoolRegisterForCleanup(self->pool, model, (auto_destruct)CogModelFree);
  CogHashSet(self->resources.models, model->name->data, model);
}

CogMesh CogBundleGetCogMesh(CogBundle self, const char* name) {
  return CogHashGet(self->resources.meshes, name);
}
CogShader CogBundleGetShader(CogBundle self, const char* name) {
  return CogHashGet(self->resources.shaders, name);
}
CogLight CogBundleGetLight(CogBundle self, const char* name) {
  return CogHashGet(self->resources.lights, name);
}
CogCamera CogBundleGetCogCamera(CogBundle self, const char* name) {
  return CogHashGet(self->resources.cameras, name);
}
CogSkeleton CogBundleGetSkeleton(CogBundle self, const char* name) {
  return CogHashGet(self->resources.skeletons, name);
}
CogImage CogBundleGetCogImage(CogBundle self, const char* name) {
  return CogHashGet(self->resources.images, name);
}
CogFont CogBundleGetFont(CogBundle self, const char* name) {
  return CogHashGet(self->resources.fonts, name);
}
CogAudioSource CogBundleGetCogAudioSource(CogBundle self, const char* name) {
  return CogHashGet(self->resources.audioSources, name);
}
CogTexture CogBundleGetTexture(CogBundle self, const char* name) {
  return CogHashGet(self->resources.textures, name);
}
CogMaterial CogBundleGetMaterial(CogBundle self, const char* name) {
  return CogHashGet(self->resources.materials, name);
}
CogModel CogBundleGetModel(CogBundle self, const char* name) {
  return CogHashGet(self->resources.models, name);
}
