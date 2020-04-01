#ifndef _COG_PRIMITIVE_H_
#define _COG_PRIMITIVE_H_

#include "cogstring.h"
#include "CogArray.h"
#include "CogHash.h"

#include "coggl.h"
#include "CogVector2.h"
#include "CogVector3.h"
#include "cogvector4.h"
#include "cograwmesh.h"

struct CogBundle;
typedef enum CogMeshDrawMethod{
  CogMeshDrawMethodPoints,
  CogMeshDrawMethodLines,
  CogMeshDrawMethodLineStrip,
  CogMeshDrawMethodLineLoop,
  CogMeshDrawMethodTriangles,
  CogMeshDrawMethodTriangleStrip,
  CogMeshDrawMethodTriangleFan
} CogMeshDrawMethod;


typedef enum ComponentSize {
  Component1D = 1,
  Component2D = 2,
  Component3D = 3,
  Component4D = 4
} ComponentSize;


typedef enum CogMeshUsage {
  CogMeshUsageStaticDraw    = GL_STATIC_DRAW,
  CogMeshUsageDynamicDraw   = GL_DYNAMIC_DRAW,
  CogMeshUsageStreamDraw    = GL_STREAM_DRAW
} CogMeshUsage;


struct CogMeshBuffer {
  CogArray(float)      data;
  ComponentSize     componentSize;
  GLuint            id;
};
typedef struct CogMeshBuffer* CogMeshBuffer;
CogDefineArray(CogMeshBuffer);

struct CogMesh {
  CogString                  name;
  CogArray(CogMeshBuffer)       buffers;
  GLuint                  id;
  CogMeshDrawMethod          drawMethod;
  CogMeshUsage               drawUsage;
  int                     numElements;
};
typedef struct CogMesh* CogMesh;
CogDefineHash(CogMesh);


CogMesh CogMeshAlloc(const char* name, struct CogBundle* CogBundle);
CogMesh CogMeshAllocPrimitiveQuad(const char* name, const float width, const float height, struct CogBundle* CogBundle);
CogMesh CogMeshAllocPrimitiveTetrahedron(const char* name, const float width, struct CogBundle* CogBundle);
CogMesh CogMeshAllocPrimitiveBox(const char* name, const float width, const float height, const float depth, struct CogBundle* CogBundle);
CogMesh CogMeshAllocPrimitiveSphere(const char* name, const float radius, const int rings, const int meridians, struct CogBundle* CogBundle);
CogMesh CogMeshAllocPrimitiveCylinder(const char* name, const float radius, const float height, struct CogBundle* CogBundle);
CogMesh CogMeshAllocPrimitiveCone(const char* name, const float radius, const float height, struct CogBundle* CogBundle);
CogMesh CogMeshAllocPrimitiveCapsule(const char* name, const float radius, const float height, struct CogBundle* CogBundle);
void CogMeshInspect(const CogMesh self);
void CogMeshFree(CogMesh self);

void CogMeshCompile(CogMesh self);
void CogMeshCompileFromRaw(CogMesh self, const CogRawMesh rawCogMesh);
void CogMeshAddBuffer(CogMesh self, const ComponentSize componentSize);

void CogMeshAddComponent1(CogMesh self, const int buffer, const float component);
void CogMeshAddComponent2(CogMesh self, const int buffer, const CogVector2 component);
void CogMeshAddComponent3(CogMesh self, const int buffer, const CogVector3 component);
void CogMeshAddComponent4(CogMesh self, const int buffer, const CogVector4 component);

#endif