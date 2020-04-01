#ifndef _COG_RAW_MESH_H_
#define _COG_RAW_MESH_H_

#include "CogVector3.h"
#include "CogVector2.h"
#include "cogtypes.h"
#include "cogstring.h"
#include "CogJsonObject.h"

struct CogBundle;

struct CogVertex {
  CogVector3   position;
  CogVector3   normal;
  CogVector3   tangent;
  int       bone[4];
  float     weight[4];
};
typedef struct CogVertex CogVertex;
CogDefineArray(CogVertex);
CogDefineList(CogVertex);
CogDefineHash(CogVertex);

struct CogPolygon {
  int       vertices[3];
  int       texCoords[3];
  CogVector3   normal;
  CogVector3   tangent;
};
typedef struct CogPolygon CogPolygon;
CogDefineArray(CogPolygon);
CogDefineList(CogPolygon);
CogDefineHash(CogPolygon);


struct CogRawMesh {
  CogString                name;
  
  Bool                  hasNormals;
  Bool                  hasTangents;
  Bool                  hasTextureCoords;
  Bool                  hasBones;
  
  CogArray(CogVector2)        texCoords;
  CogArray(CogVertex)         vertices;
  CogArray(CogPolygon)     polygons;
};
typedef struct CogRawMesh* CogRawMesh;
CogDefineArray(CogRawMesh);
CogDefineHash(CogRawMesh);


CogRawMesh     CogRawMeshAlloc(const char* name, struct CogBundle* CogBundle);
CogRawMesh     CogRawMeshAllocPrimitiveQuad(const char* name, const float width, const float height, struct CogBundle* CogBundle);
CogRawMesh     CogRawMeshAllocPrimitiveTetrahedron(const char* name, const float width, struct CogBundle* CogBundle);
CogRawMesh     CogRawMeshAllocPrimitiveBox(const char* name, const float width, const float height, const float depth, struct CogBundle* CogBundle);
CogRawMesh     CogRawMeshAllocPrimitiveSphere(const char* name, const float radius, const int rings, const int meridians, struct CogBundle* CogBundle);
CogRawMesh     CogRawMeshAllocPrimitiveCylinder(const char* name, const float radius, const float height, struct CogBundle* CogBundle);
CogRawMesh     CogRawMeshAllocPrimitiveCone(const char* name, const float radius, const float height, struct CogBundle* CogBundle);
CogRawMesh     CogRawMeshAllocPrimitiveCapsule(const char* name, const float radius, const float height, struct CogBundle* CogBundle);
CogRawMesh     CogRawMeshAllocFromJSON(CogJsonObject json, struct CogBundle* CogBundle);
CogJsonObject  CogRawMeshToJson(const CogRawMesh self);
void        CogRawMeshInspect(const CogRawMesh self);
void        CogRawMeshFree(CogRawMesh self);
void        CogRawMeshRecalculateNormals(CogRawMesh self);
void        CogRawMeshRecalculateTangents(CogRawMesh self);

#endif