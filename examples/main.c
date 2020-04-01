#include <stdio.h>
#include <cog.h>
#include <math.h>

struct ExampleScene {
  CogInherit(struct CogScene);
  
  CogBundle       localStorage;
  CogTexture      gimp, blender;
  CogShader       shader, blit, deferredCompose, dbgPosition, dbgNormal, dbgDepth, dbgCogColor;
  
  CogModel        quad;
  
  CogNode         pos, yaw, lightnode, spotnode, spotLookat, spotLookat2, dirnode;
  CogNode         lvl1, lvl2;
  
  CogAudioSource  pjesma;
  
};
typedef struct ExampleScene* ExampleScene;
static CogNode nodes[32][32][2];

Bool update(CogScene self, double dt) {
  ExampleScene s = (ExampleScene)self;
  if( CogKeyboardKeyReleased(CogKeyEscape) ) {
    printf("Released Key Escape\n");
    return False;
  }
  
  if( CogKeyboardKeyDown(CogKeyW) ) { CogNodeTranslateForward(s->pos, dt); }
  if( CogKeyboardKeyDown(CogKeyS) ) { CogNodeTranslateBackward(s->pos, dt); }
  if( CogKeyboardKeyDown(CogKeyD) ) { CogNodeTranslateRight(s->pos, dt); }
  if( CogKeyboardKeyDown(CogKeyA) ) { CogNodeTranslateLeft(s->pos, dt); }
  if( CogKeyboardKeyDown(CogKeySpace) ) { CogNodeTranslateUp(s->pos, dt); }
  if( CogKeyboardKeyDown(CogKeyLCtrl) ) { CogNodeTranslateDown(s->pos, dt); }
  
  if( CogKeyboardKeyDown(CogKeyLeft) ) { CogNodeTranslateLeft(s->spotnode, 3.0f * dt); }
  if( CogKeyboardKeyDown(CogKeyRight) ) { CogNodeTranslateRight(s->spotnode, 3.0f * dt); }
  if( CogKeyboardKeyDown(CogKeyUp) ) { CogNodeTranslateUp(s->spotnode, 3.0f * dt); }
  if( CogKeyboardKeyDown(CogKeyDown) ) { CogNodeTranslateDown(s->spotnode, 3.0f * dt); }
  
  if( CogKeyboardKeyDown(CogKeyM) ) { ((CogSpotLight)s->super.lights->data[3])->innerAngle += 1.0f * dt; }
  if( CogKeyboardKeyDown(CogKeyK) ) { ((CogSpotLight)s->super.lights->data[3])->outerAngle += 1.0f * dt; }
  if( CogKeyboardKeyDown(CogKeyN) ) { ((CogSpotLight)s->super.lights->data[3])->innerAngle -= 1.0f * dt; }
  if( CogKeyboardKeyDown(CogKeyJ) ) { ((CogSpotLight)s->super.lights->data[3])->outerAngle -= 1.0f * dt; }
  
  /*
  if( CogKeyboardKeyPressed(CogKeyP) ) { CogAudioSourcePause(s->pjesma); }
  if( CogKeyboardKeyPressed(CogKeyO) ) { CogAudioSourcePlay(s->pjesma); }
  if( CogKeyboardKeyPressed(CogKeyI) ) { CogAudioSourceStop(s->pjesma); }
  if( CogKeyboardKeyPressed(CogKeyU) ) { CogAudioSourceResume(s->pjesma); }
  */
  
  /*
  Clampf(&((SpotLight)s->super.lights->data[3])->outerAngle, 0.0f, 1.0f);
  Clampf(&((SpotLight)s->super.lights->data[3])->innerAngle, 0.0f, 1.0f);
  */
  static float camPitch = 0.0f;
  static float camYaw = 0.0f;
  
  if(CogMouseMoved() && CogMouseButtonDown(CogMouseButtonLeft)) {
    CogVector2 offset = CogMouseGetRelativePosition();
    camYaw += offset.x * dt * 5.0f;
    camPitch += offset.y * dt * 5.0f;
  }
  
  static float angle = 0.0f;
  angle += 0.2f * dt;
  
  int i, j;
  for(i = 0; i < 32; ++i) {
    for(j = 0; j < 32; ++j) {
      float l = sqrt(i*i/32.0f/32.0f + j*j/32.0f/32.0f);
      nodes[i][j][0]->position.y = (1.0f + (cos(l * angle) * sin(l * angle))) * 0.2f;
      //nodes[i][j][0]->position.x = (1.0f + (sin(cos(l*l * angle)) * sin(l * angle)) + cos(sin(angle) + cos(angle * 7) * sin(l *l *l))) * 1.2f;
      //nodes[i][j][0]->position.z = (1.0f + (cos(l*angle + angle) * cos(sin(l * angle*angle)))) * 1.2f;
      CogNodeYaw(nodes[i][j][1], sin(angle * l));
    }
  }
  
  CogNodeUpdateTransforms(s->super.sceneGraph->root);
  CogNodeSetRotatation(s->yaw, (CogVector3){1.0f, 0.0f, 0.0f}, camPitch);
  CogNodeSetRotatation(s->pos, (CogVector3){0.0f, 1.0f, 0.0f}, camYaw);
  
  CogNodeSetRotatation(s->lvl2, (CogVector3){0.0f, 1.0f, 1.0f}, angle * 50.0f);
  
  //CogNodeSetTranslation(s->spotnode, (CogVector3){s->yaw->model.vector.w.x, s->yaw->model.vector.w.y, s->yaw->model.vector.w.z});
  CogNodeLookAt(s->spotLookat, (CogVector3){2.0f * sinf(angle), 0.0f, 2.0f * cosf(angle)}, (CogVector3){0.0f, 1.0f, 0.0f});
  CogNodeLookAt(s->spotLookat2, (CogVector3){2.0f * cosf(angle * 0.9f), 0.0f, 2.0f * sinf(angle * 0.9f)}, (CogVector3){0.0f, 1.0f, 0.0f});
  
  CogNodeSetTranslation(s->lightnode, (CogVector3){2.0f * sinf(angle), 0.5f, 2.0f * cosf(angle)});
  
  CogSceneGraphUpdate(self->sceneGraph, dt);
  return True;
}
  
void cleanup(CogScene self) {
  
}

ExampleScene ExampleSceneAlloc(void) {
  ExampleScene self = malloc(sizeof(struct ExampleScene));
  CogSceneInitBase((CogScene)self);
  
  self->super.name = CogStringAlloc("Example Scene");
  self->super.update = update;
  self->super.cleanup = cleanup;
  
  self->localStorage = CogResourceManagerAddBundle(CogBundleAlloc("Example Scene/Local Storage"));
  self->gimp = CogTextureAllocFromFile("Gimp CogImage", "./builtins/images/gimp.png", self->localStorage);
  self->blender = CogTextureAllocFromFile("Blender CogImage", "./builtins/images/blender.png", self->localStorage);
  
  CogTexture normalMap = CogTextureAllocFromFile("Normal Map Image", "./builtins/images/NormalMap.png", self->localStorage);
  CogTexture diffuseMap = CogTextureAllocFromFile("Diffuse Map Image", "./builtins/images/crate.png", self->localStorage);
  CogTexture randomMap = CogTextureAllocFromFile("Random Normal Map Image", "./builtins/images/randomnormalmap.png", self->localStorage);
  
  CogTexture skullNormalMap = CogTextureAllocFromFile("Normal Map Image", "./builtins/images/skull_normalmap.png", self->localStorage);
  CogTexture skullDiffuseMap = CogTextureAllocFromFile("Normal Map Image", "./builtins/images/skull_skin.png", self->localStorage);
  
  CogCameraSetSize(self->super.activeCamera, 1920.0f, 1080.0f);
  CogCameraSetNearFar(self->super.activeCamera, 0.01f, 100.0f);
  
  CogMaterial tmp = CogMaterialAlloc("Quad Material", NULL, self->localStorage);
  tmp->diffuse.diffuseMap = diffuseMap;
  tmp->diffuse.diffuseIntensity = 1.0f;
  tmp->diffuse.diffuseColor = (CogColor){1.0f, 1.0f, 1.0f, 1.0f};
  tmp->specular.specularIntensity = 0.3f;
  tmp->specular.specularHardness = 12.0f;
  tmp->specular.specularColor = (CogColor){0.3f, 0.3f, 0.3f, 1.0f};
  tmp->bump.normalMap = normalMap;
  
  CogMaterial skullMaterial = CogMaterialAlloc("Skull Material", NULL, self->localStorage);
  //skullMaterial->diffuse.diffuseMap = skullDiffuseMap;
  skullMaterial->diffuse.diffuseIntensity = 1.0f;
  skullMaterial->diffuse.diffuseIntensity = 1.0f;
  skullMaterial->diffuse.diffuseColor = (CogColor){1.0f, 1.0f, 1.0f, 1.0f};
  skullMaterial->specular.specularIntensity = 0.3f;
  skullMaterial->specular.specularHardness = 12.0f;
  skullMaterial->specular.specularColor = (CogColor){0.8f, 0.8f, 0.8f, 1.0f};
  skullMaterial->bump.normalMap = skullNormalMap;
  
  //CogJsonObject mesh = JsonParserParseFromFile(NULL, "./builtins/meshes/cube.json");
  CogJsonObject mesh = CogJsonParserParseFromFile(NULL, "./builtins/meshes/skull.json");
  CogRawMesh msh = CogRawMeshAllocFromJSON(mesh, self->localStorage);
  if(!msh->hasNormals) { CogRawMeshRecalculateNormals(msh); }
  if(!msh->hasTangents) { CogRawMeshRecalculateTangents(msh); }
  CogMesh m = CogMeshAlloc("Testni", self->localStorage);
  CogMeshCompileFromRaw(m, msh);
  CogModel kocka = CogModelAlloc("Kocka", m, skullMaterial, NULL, self->localStorage);
  
  CogRawMesh quad = CogRawMeshAllocPrimitiveQuad("Quad", 1.0f, 1.0f, self->localStorage);
  CogMesh quadMsh = CogMeshAlloc("Testni Quad", self->localStorage);
  CogMeshCompileFromRaw(quadMsh, quad);
  
  self->quad = CogModelAlloc("Quad Model", quadMsh, tmp, NULL, self->localStorage);
  
  self->lvl1 = CogNodeAlloc("Lvl 1 CogNode", self->super.sceneGraph->root);
  self->lvl2 = CogNodeAlloc("Lvl 2 CogNode", self->lvl1);
  //CogNodeAttachModel(self->lvl2, self->quad);
  //CogNodeAttachModel(self->lvl2, kocka);
  
  self->pos = CogNodeAlloc("Cam Pos CogNode", self->super.sceneGraph->root);
  self->yaw = CogNodeAlloc("Cam Yaw CogNode", self->pos);
  CogNodeAttachCamera(self->yaw, self->super.activeCamera);
  
  CogNode hook = CogNodeAlloc("Hook", self->super.sceneGraph->root);
  CogNodeScale(hook, 10, 1, 10);
  CogNodeAttachModel(hook, self->quad);
  //CogNodeRotate(hook, (CogVector3){1.0f, 0.0f, 0.0f}, 90.0f);
  
  CogNodeTranslate(self->lvl1, (CogVector3){2.0f, 0.0f, 0.0f});
  CogNodeTranslate(self->lvl2, (CogVector3){0.0f, 1.0f, 0.0f});
  CogNodeTranslate(self->pos, (CogVector3){0.0f, 3.0f, 3.0f});
  
  CogNodeUpdateTransforms(self->super.sceneGraph->root);
  CogNodeLookAt(self->yaw, (CogVector3){0.0f, 0.0f, 0.0f}, (CogVector3){0.0f, 1.0f, 0.0f});
  
  // lightnode
  self->lightnode = CogNodeAlloc("Light CogNode", self->super.sceneGraph->root);
  CogNodeSetTranslation(self->lightnode, (CogVector3){0.0f, 0.1f, 0.0f});
  
  self->dirnode = CogNodeAlloc("Directional Light CogNode", self->super.sceneGraph->root);
  CogNodeRotate(self->dirnode, (CogVector3){1.0f, 0.0f, 0.0f}, -135.0f);
  
  self->spotnode = CogNodeAlloc("Spot Light CogNode", self->super.sceneGraph->root);
  self->spotLookat = CogNodeAlloc("Tmp", self->spotnode);
  
  CogNodeSetTranslation(self->spotnode, (CogVector3){-4.0f, 6.0f, 0.0f});
  CogNodeUpdateTransforms(self->super.sceneGraph->root);
  
  CogNode tmpn = CogNodeAlloc("VBla", self->super.sceneGraph->root);
  self->spotLookat2 = CogNodeAlloc("Bla", tmpn);
  CogNodeSetTranslation(tmpn, (CogVector3){-4.0f, 6.0f, 0.0f});
  
  int i, j;
  for(i = 0; i < 32; ++i) {
    for(j = 0; j < 32; ++j) {
      CogString posnodename = CogStringAllocFromFormat("skull_node_position_%d_%d", i, j);
      CogString scalenodename = CogStringAllocFromFormat("skull_node_scale_%d_%d", i, j);
      
      CogNode pos = CogNodeAlloc(c_str(posnodename), self->super.sceneGraph->root);
      CogNode scale = CogNodeAlloc(c_str(scalenodename), pos);
      
      CogStringFree(posnodename);
      CogStringFree(scalenodename);
      
      CogNodeSetTranslation(pos, (CogVector3){4.0f - i * 0.25f, 0.08f, 4.0f - j * 0.25f});
      CogNodeSetScale(scale, 0.04f, 0.04f, 0.04f);
      
      CogNodeAttachModel(scale, kocka);
      nodes[i][j][0] = pos;
      nodes[i][j][1] = scale;
    }
  }
  
  // point light
  CogPointLight light = CogPointLightAlloc("Point Light Red", (CogScene)self, self->lightnode, NULL, NULL, 0, self->localStorage);
  light->constantAttenuation = 0.51f;
  light->linearAttenuation = 0.42f;
  light->quadraticAttenuation = 0.44f;
  light->super.color = (CogColor){0.5f, 0.7f, 0.7f, 1.0f};
  
  // directional light
  CogDirectionalLight directionalLight = CogDirectionalLightAlloc("Directional Light Yellow", (CogScene)self, self->dirnode, NULL, NULL, 1, self->localStorage);
  directionalLight->super.color = (CogColor){0.5f, 0.5f, 0.5f, 1.0f};
  
  // spot light
  CogSpotLight spotLight = CogSpotLightAlloc("Spot Light White", (CogScene)self, self->spotLookat, NULL, NULL, -2, self->localStorage);
  spotLight->super.color = (CogColor){0.5f, 0.5f, 0.5f, 1.0f};
  spotLight->constantAttenuation = 0.01f;
  spotLight->linearAttenuation = 0.02f;
  spotLight->quadraticAttenuation = 0.4f;
  spotLight->innerAngle = 0.05f;
  spotLight->outerAngle = 0.09f;
  
  // spot light
  CogSpotLight spotLight2 = CogSpotLightAlloc("Spot Light Green", (CogScene)self, self->spotLookat2, NULL, NULL, -3, self->localStorage);
  //CogSpotLight spotLight2 = CogSpotLightAlloc("Spot Light Green", (CogScene)self, self->yaw, NULL, NULL, 3, self->localStorage);
  spotLight2->super.color = (CogColor){0.1f, 0.1f, 0.1f, 1.0f};
  spotLight2->constantAttenuation = 0.51f;
  spotLight2->linearAttenuation = 0.92f;
  spotLight2->quadraticAttenuation = 3.4f;
  spotLight2->innerAngle = 0.01f;
  spotLight2->outerAngle = 0.012f;
  
  // ambient light
  CogAmbientLight ambientLight = CogAmbientLightAlloc("Ambient Light Blue", (CogScene)self, self->lightnode, 2, self->localStorage);
  ambientLight->super.color = (CogColor){0.0f, 0.0f, 0.0f, 1.0f};
  
  /*
  self->pjesma = CogAudioSourceAllocFromFile("", "./builtins/music/espada.mp3", NULL);
  CogAudioSourcePlay(self->pjesma);
  */
  
  return self;
}

#include "cogpair.h"
#include "cogmap.h"

CogPairDefine(double, CogVector2);
CogMapDefine(double, CogVector2);

int main(int argc, char **argv) {
  
  CogVector2 v = {1,2};
  CogMap(double, CogVector2) map = CogMapAlloc(double, CogVector2);
  CogMapAdd(double, CogVector2, map, 1, v);
  CogMapInspect(double, CogVector2, map);
  
  // Initialize Cog
  CogInit();
  /*
  Mix_Init(MIX_INIT_MP3);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  Mix_AllocateChannels(0);
  */
  
  // Fetch all displays available and create a window in one of those
  CogArray(CogScreen) screens = CogPlatformGetScreens();
  CogPlatformCreateWindow("Example Window", (CogRect){.x = 0, .y = 0, .w = 1920 / 2, .h = 1080 / 2}, screens->data[0], False, False, False);
  
  // Create a renderer
  CogRendererDeferredConfiguration conf = {};
  conf.debug.debugShaderOutput = True;
  conf.resolution.width = 1920 / 2;
  conf.resolution.height = 1080 / 2;
  conf.deferred.useFP32PositionTarget = True;
  conf.screenSpace.scale = 1.0f;
  conf.bloom.scale = 1.0f;
  
  CogRendererDeferred rnd = CogRendererDeferredAlloc(conf);
  
  /* Set video options */
  rnd->videoOptions.edgeDetect.on = False;
  rnd->videoOptions.ssaa.on = False;
  rnd->videoOptions.ssr.on = False;
  
  rnd->videoOptions.ssao.on = True;
  rnd->videoOptions.ssao.bias = 0.02f;
  rnd->videoOptions.ssao.constantAttenuation = 2.0f;
  rnd->videoOptions.ssao.linearAttenuation = 2.0f;
  rnd->videoOptions.ssao.quadraticAttenuation = 1.0f;
  rnd->videoOptions.ssao.numSamples = 128;
  rnd->videoOptions.ssao.radius = 5.0f;
  rnd->videoOptions.ssao.blur.on = True;
  rnd->videoOptions.ssao.blur.numSamples = 9;
  rnd->videoOptions.ssao.blur.radius = 3.0f;
  rnd->videoOptions.bloom.on = False;
  
  CogRenderSystemSetRenderer((CogRenderer)rnd); 
  
  // Create a scene, add it to scenemanager and activate it
  ExampleScene scene = ExampleSceneAlloc();
  CogSceneManagerAddScene( (CogScene)scene );
  CogSceneManagerActivateScene("Example Scene");
  
  // Start the Cog main-loop
  CogStart();
  
  // Cleanup all objects and resources allocated by Cog
  CogCleanUp();

  return 0;
}