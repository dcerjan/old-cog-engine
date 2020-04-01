#ifndef _COG_MESH_H_
#define _COG_MESH_H_

#include "cogstring.h"
#include "CogHash.h"

#include "cogmesh.h"
#include "cogmaterial.h"
#include "cogskeleton.h"

struct CogBundle;
struct CogModel {
  CogString      name;
  CogMesh        mesh;
  CogMaterial    material;
  CogSkeleton    skeleton;
};
typedef struct CogModel* CogModel;
CogDefineHash(CogModel);
CogDefineList(CogModel);
CogDefineArray(CogModel);

CogModel CogModelAlloc(const char* name, CogMesh mesh, CogMaterial material, CogSkeleton skeleton, struct CogBundle* bundle);
void CogModelInspect(const CogModel self);
void CogModelFree(CogModel self);

void CogModelUpdate(CogModel self, double t);
void CogModelPlayAnimation(CogModel self, const char* animation, const float advanceSpeed,const Bool loop);

#endif