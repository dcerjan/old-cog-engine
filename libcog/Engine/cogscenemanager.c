#include "cogscenemanager.h"
#include "cogscene.h"
#include "coggc.h"
#include "cogrendersystem.h"
#include "cogrenderer.h"

static struct {
  CogHash(CogScene)   scenes;
  CogScene            activeScene;
  CogCleanupPool      pool;
} __cog_scene_manager__;

void CogSceneManagerInit(void) {
  __cog_scene_manager__.activeScene = NULL;
  __cog_scene_manager__.pool = CogCleanupPoolAlloc("Scene Manager Cleanup Pool");
  __cog_scene_manager__.scenes = CogHashAlloc(CogScene);
}

struct CogScene* CogSceneManagerGetScene(const char* name) {
  return CogHashGet(__cog_scene_manager__.scenes, name);
}

void CogSceneManagerActivateScene(const char* name) {
  __cog_scene_manager__.activeScene = CogHashGet(__cog_scene_manager__.scenes, name);
}

void CogSceneManagerRemoveScene(const char* name) {
  CogScene toDelete = NULL;
  CogListForEach(__cog_scene_manager__.pool->cleanupPool, e, {
    if( !strcmp( ((CogScene)e.object)->name->data, name) ) {
        toDelete = (CogScene)e.object;
    }
  });
  if(toDelete) {
    CogListRemoveEach(__cog_scene_manager__.pool->cleanupPool, e, {
      !strcmp( ((CogScene)e.object)->name->data, name);
    });
    CogHashRemove(__cog_scene_manager__.scenes, name);
    if(toDelete == __cog_scene_manager__.activeScene) {
      __cog_scene_manager__.activeScene = NULL;
    }
    toDelete->cleanup(toDelete);
  }
}

void CogSceneManagerAddScene(struct CogScene* scene) {
  CogCleanupPoolRegisterForCleanup(__cog_scene_manager__.pool, scene, (auto_destruct)scene->cleanup);
  CogHashSet(__cog_scene_manager__.scenes, scene->name->data, scene);
}

Bool CogSceneManagerUpdate(double dt) {
  if(__cog_scene_manager__.activeScene) {
    if(!__cog_scene_manager__.activeScene->update(__cog_scene_manager__.activeScene, dt)) {
      return False;
    }
    if(__cog_scene_manager__.activeScene->lateUpdate && !__cog_scene_manager__.activeScene->lateUpdate(__cog_scene_manager__.activeScene, dt)) {
      return False;
    }
    return True;
  }
  return False;
}

void CogSceneManagerDraw(void) {
  if(__cog_scene_manager__.activeScene) {
    
    if(__cog_scene_manager__.activeScene->draw) {
      __cog_scene_manager__.activeScene->draw(__cog_scene_manager__.activeScene);
    } else {
      CogRenderSystemGetRenderer()->renderScene(CogRenderSystemGetRenderer(), __cog_scene_manager__.activeScene);
    }
  }
}

void CogSceneManagerCleanUp(void) {
  __cog_scene_manager__.activeScene = NULL;
  CogCleanupPoolFree(__cog_scene_manager__.pool);
  CogHashFree(__cog_scene_manager__.scenes);
}

struct CogScene* CogSceneManagerGetActiveScene(void) {
  return __cog_scene_manager__.activeScene;
}