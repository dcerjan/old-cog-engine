#ifndef _COG_BUNDLE_H_
#define _COG_BUNDLE_H_

#include "coggc.h"
#include "cogmesh.h"
#include "cograwmesh.h"
#include "cogmaterial.h"
#include "cogskeleton.h"
#include "cogmodel.h"
#include "cogshader.h"
#include "coglight.h"
#include "cogtexture.h"
#include "cogimage.h"
#include "cogaudiosource.h"
#include "cogfont.h"
#include "cogcamera.h"
#include "coggc.h"

struct CogBundle {
  CogString                 name;
  struct {
    
    // raw resources
    CogHash(CogMesh)          meshes;
    CogHash(CogRawMesh)       rawMeshes;
    CogHash(CogShader)        shaders;
    CogHash(CogLight)         lights;
    CogHash(CogCamera)        cameras;
    CogHash(CogSkeleton)      skeletons;
    CogHash(CogImage)         images;
    CogHash(CogAudioSource)   audioSources;
    CogHash(CogFont)          fonts;
    
    // baked resources
    CogHash(CogTexture)       textures;
    CogHash(CogMaterial)      materials;
    CogHash(CogModel)         models;
  } resources;
  CogCleanupPool            pool;
};
typedef struct CogBundle* CogBundle;
CogDefineHash(CogBundle);

CogBundle CogBundleAlloc(const char* name);
void CogBundleInspect(const CogBundle self);
void CogBundleFree(CogBundle self);

void            CogBundleAddMesh          (CogBundle self, const CogMesh mesh);
void            CogBundleAddRawMesh       (CogBundle self, const CogRawMesh rawCogMesh);
void            CogBundleAddShader        (CogBundle self, const CogShader shader);
void            CogBundleAddLight         (CogBundle self, const CogLight light);
void            CogBundleAddCamera        (CogBundle self, const CogCamera CogCamera);
void            CogBundleAddSkeleton      (CogBundle self, const CogSkeleton skeleton);
void            CogBundleAddImage         (CogBundle self, const CogImage image);
void            CogBundleAddFont          (CogBundle self, const CogFont font);
void            CogBundleAddAudioSource   (CogBundle self, const CogAudioSource audioSource);
void            CogBundleAddTexture       (CogBundle self, const CogTexture texture);
void            CogBundleAddMaterial      (CogBundle self, const CogMaterial material);
void            CogBundleAddModel         (CogBundle self, const CogModel model);

CogMesh         CogBundleGetMesh          (CogBundle self, const char* name);
CogRawMesh      CogBundleGetRawMesh       (CogBundle self, const char* name);
CogShader       CogBundleGetShader        (CogBundle self, const char* name);
CogLight        CogBundleGetLight         (CogBundle self, const char* name);
CogCamera       CogBundleGetCamera        (CogBundle self, const char* name);
CogSkeleton     CogBundleGetSkeleton      (CogBundle self, const char* name);
CogImage        CogBundleGetImage         (CogBundle self, const char* name);
CogFont         CogBundleGetFont          (CogBundle self, const char* name);
CogAudioSource  CogBundleGetAudioSource   (CogBundle self, const char* name);
CogTexture      CogBundleGetTexture       (CogBundle self, const char* name);
CogMaterial     CogBundleGetMaterial      (CogBundle self, const char* name);
CogModel        CogBundleGetModel         (CogBundle self, const char* name);

#endif