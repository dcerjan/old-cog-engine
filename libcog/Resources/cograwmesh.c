#include "cograwmesh.h"
#include "cogbundle.h"
#include "cogplatform.h"

void CogPolygonInspect(const CogPolygon self) {
  printf("<CogPolygon Object\n");
  printf("  normal: "); CogVector3Inspect(self.normal);
  printf("  tangent: "); CogVector3Inspect(self.tangent);
  printf("  texCoords: [%d, %d, %d]\n", self.texCoords[0], self.texCoords[1], self.texCoords[2]);
  printf("  vertices: [%d, %d, %d]\n", self.vertices[0], self.vertices[1], self.vertices[2]);
  printf(">\n");
}

void CogVertexInspect(const CogVertex self) {
  printf("<CogVertex Object\n");
  printf("  bones: [%d, %d, %d, %d]\n", self.bone[0], self.bone[1], self.bone[2], self.bone[3]);
  printf("  weights: [%f, %f, %f, %f]\n", self.weight[0], self.weight[1], self.weight[2], self.weight[3]);
  printf("  position: "); CogVector3Inspect(self.position);
  printf("  normal: "); CogVector3Inspect(self.normal);
  printf("  tangent: "); CogVector3Inspect(self.tangent);
  printf(">\n");
}

CogRawMesh CogRawMeshAlloc(const char* name, struct CogBundle* bundle) {
  CogRawMesh self = malloc(sizeof(struct CogRawMesh));
  self->name = CogStringAlloc(name);
  
  self->vertices = CogArrayAlloc(CogVertex);
  self->polygons = CogArrayAlloc(CogPolygon);
  self->texCoords = CogArrayAlloc(CogVector2);
  
  self->hasNormals = True;
  self->hasTangents = True;
  self->hasTextureCoords = True;
  self->hasBones = True;
  
  if(bundle) {
    CogBundleAddCogRawMesh(bundle, self);
  }
  
  return self;
}

CogRawMesh CogRawMeshAllocPrimitiveQuad(const char* name, const float width, const float height, struct CogBundle* CogBundle) {
  CogRawMesh self = CogRawMeshAlloc(name, CogBundle);
  
  self->hasBones = False;
  
  CogVertex v0 = (CogVertex){
    .bone = {0, 0, 0, 0},
    .weight = {0.0f, 0.0f, 0.0f, 0.0f},
    .position = (CogVector3){-width * 0.5f, 0.0f, -height * 0.5f},
    .normal = (CogVector3){0.0f, 1.0f, 0.0f},
    .tangent = (CogVector3){1.0f, 0.0f, 0.0f}
  };
  
  CogVertex v1 = (CogVertex){
    .bone = {0, 0, 0, 0},
    .weight = {0.0f, 0.0f, 0.0f, 0.0f},
    .position = (CogVector3){-width * 0.5f, 0.0f, height * 0.5f},
    .normal = (CogVector3){0.0f, 1.0f, 0.0f},
    .tangent = (CogVector3){1.0f, 0.0f, 0.0f}
  };
  
  CogVertex v2 = (CogVertex){
    .bone = {0, 0, 0, 0},
    .weight = {0.0f, 0.0f, 0.0f, 0.0f},
    .position = (CogVector3){width * 0.5f, 0.0f, height * 0.5f},
    .normal = (CogVector3){0.0f, 1.0f, 0.0f},
    .tangent = (CogVector3){1.0f, 0.0f, 0.0f}
  };
  
  CogVertex v3 = (CogVertex){
    .bone = {0, 0, 0, 0},
    .weight = {0.0f, 0.0f, 0.0f, 0.0f},
    .position = (CogVector3){width * 0.5f, 0.0f, -height * 0.5f},
    .normal = (CogVector3){0.0f, 1.0f, 0.0f},
    .tangent = (CogVector3){1.0f, 0.0f, 0.0f}
  };
  
  CogArrayPush(self->vertices, v0);
  CogArrayPush(self->vertices, v1);
  CogArrayPush(self->vertices, v2);
  CogArrayPush(self->vertices, v3);
  
  CogVector2 uv0 = (CogVector2){.x = 0.0f, .y = 0.0f};
  CogVector2 uv1 = (CogVector2){.x = 0.0f, .y = 1.0f};
  CogVector2 uv2 = (CogVector2){.x = 1.0f, .y = 1.0f};
  CogVector2 uv3 = (CogVector2){.x = 1.0f, .y = 0.0f};
  
  CogArrayPush(self->texCoords, uv0);
  CogArrayPush(self->texCoords, uv1);
  CogArrayPush(self->texCoords, uv2);
  CogArrayPush(self->texCoords, uv3);
  
  CogPolygon p0 = (CogPolygon){
    .normal = (CogVector3){0.0f, 1.0f, 0.0f},
    .tangent = (CogVector3){0.0f, 0.0f, 0.0f},
    .texCoords = {0, 1, 2},
    .vertices = {0, 1, 2}
  };
  
  CogPolygon p1 = (CogPolygon){
    .normal = (CogVector3){0.0f, 1.0f, 0.0f},
    .tangent = (CogVector3){0.0f, 0.0f, 0.0f},
    .texCoords = {0, 2, 3},
    .vertices = {0, 2, 3}
  };
  
  CogArrayPush(self->polygons, p0);
  CogArrayPush(self->polygons, p1);
  
  self->hasTangents = True;
  self->hasNormals = True;
  
  return self;
}

CogRawMesh CogRawMeshAllocPrimitiveTetrahedron(const char* name, const float width, struct CogBundle* CogBundle) {
  CogRawMesh self = CogRawMeshAlloc(name, CogBundle);
  self->hasBones = False;
  return self;
}

CogRawMesh CogRawMeshAllocPrimitiveBox(const char* name, const float width, const float height, const float depth, struct CogBundle* CogBundle) {
  CogRawMesh self = CogRawMeshAlloc(name, CogBundle);
  self->hasBones = False;
  return self;
}

CogRawMesh CogRawMeshAllocPrimitiveSphere(const char* name, const float radius, const int rings, const int meridians, struct CogBundle* CogBundle) {
  CogRawMesh self = CogRawMeshAlloc(name, CogBundle);
  self->hasBones = False;
  return self;
}

CogRawMesh CogRawMeshAllocPrimitiveCylinder(const char* name, const float radius, const float height, struct CogBundle* CogBundle) {
  CogRawMesh self = CogRawMeshAlloc(name, CogBundle);
  self->hasBones = False;
  return self;
}

CogRawMesh CogRawMeshAllocPrimitiveCone(const char* name, const float radius, const float height, struct CogBundle* CogBundle) {
  CogRawMesh self = CogRawMeshAlloc(name, CogBundle);
  self->hasBones = False;
  return self;
}

CogRawMesh CogRawMeshAllocPrimitiveCapsule(const char* name, const float radius, const float height, struct CogBundle* CogBundle) {
  CogRawMesh self = CogRawMeshAlloc(name, CogBundle);
  self->hasBones = False;
  return self;
}

CogRawMesh CogRawMeshAllocFromJSON(CogJsonObject json, struct CogBundle* CogBundle) {
  CogRawMesh msh = CogRawMeshAlloc(CogJsonObjectGetString(json, "name")->data, CogBundle);
  
  CogJsonObject data, material, skeleton;

  // CogMesh parsing
  data = CogJsonObjectGetObject(json, "data");
  CogArray(CogJsonObject) uvs = CogJsonObjectGetCogArray(data, "uvs");
  CogArray(CogJsonObject) vertices = CogJsonObjectGetCogArray(data, "vertices");
  CogArray(CogJsonObject) polygons = CogJsonObjectGetCogArray(data, "polygons");
  
  if(uvs) {
    CogArrayForEach(uvs, u, {
      CogArray(CogJsonObject) uv = (CogArray(CogJsonObject))u->data;
      CogVector2 texCoord;
      texCoord.x = (*(double*)uv->data[0]->data);
      texCoord.y = (*(double*)uv->data[1]->data);
      CogArrayPush(msh->texCoords, texCoord);
    });
  }
  
  CogArrayForEach(vertices, v, {
    CogArray(CogJsonObject) position = CogJsonObjectGetCogArray(v, "position");
    CogArray(CogJsonObject) normal = CogJsonObjectGetCogArray(v, "normal");
    CogArray(CogJsonObject) tangent = CogJsonObjectGetCogArray(v, "tangent");
    CogArray(CogJsonObject) weights = CogJsonObjectGetCogArray(v, "weights");
    CogArray(CogJsonObject) bones = CogJsonObjectGetCogArray(v, "bones");
    
    CogVertex vert;
    
    if(position) {
      vert.position.x = *((double*)position->data[0]->data);
      vert.position.y = *((double*)position->data[1]->data);
      vert.position.z = *((double*)position->data[2]->data);
    } else {
      CogLoggerError(CogPlatformGetLogger(), "CogMesh has to have position data!\n");
      exit(1);
    }
    
    if(normal) {
      vert.normal.x = *((double*)normal->data[0]->data);
      vert.normal.y = *((double*)normal->data[1]->data);
      vert.normal.z = *((double*)normal->data[2]->data);
    } else {
      msh->hasNormals = False;
    }
    
    if(tangent) {
      vert.tangent.x = *((double*)tangent->data[0]->data);
      vert.tangent.y = *((double*)tangent->data[1]->data);
      vert.tangent.z = *((double*)tangent->data[2]->data);
    } else {
      msh->hasTangents = False;
    }
    
    if(bones) {
      vert.bone[0] = (int)*((double*)bones->data[0]->data);
      vert.bone[1] = (int)*((double*)bones->data[1]->data);
      vert.bone[2] = (int)*((double*)bones->data[2]->data);
      vert.bone[3] = (int)*((double*)bones->data[3]->data);
    } else {
      msh->hasBones = False;
    }
    
    if(weights) {
      vert.weight[0] = (float)*((double*)weights->data[0]->data);
      vert.weight[1] = (float)*((double*)weights->data[1]->data);
      vert.weight[2] = (float)*((double*)weights->data[2]->data);
      vert.weight[3] = (float)*((double*)weights->data[3]->data);
    } else {
      msh->hasBones = False;
    }
    
    
    CogArrayPush(msh->vertices, vert);
  });

  CogArrayForEach(polygons, p, {
    
    CogArray(CogJsonObject) vertices = CogJsonObjectGetCogArray(p, "vertices");
    CogArray(CogJsonObject) normal = CogJsonObjectGetCogArray(p, "normal");
    CogArray(CogJsonObject) tangent = CogJsonObjectGetCogArray(p, "tangent");
    CogArray(CogJsonObject) uvs = CogJsonObjectGetCogArray(p, "uvs");
    
    CogPolygon poly;
    
    if(vertices) {
      poly.vertices[0] = (int)*((double*)vertices->data[0]->data);
      poly.vertices[1] = (int)*((double*)vertices->data[1]->data);
      poly.vertices[2] = (int)*((double*)vertices->data[2]->data);
    } else {
      CogLoggerError(CogPlatformGetLogger(), "CogMesh CogPolygon has to have vertex indices!\n");
      exit(1);
    }
    
    if(uvs) {
      poly.texCoords[0] = (int)*((double*)uvs->data[0]->data);
      poly.texCoords[1] = (int)*((double*)uvs->data[1]->data);
      poly.texCoords[2] = (int)*((double*)uvs->data[2]->data);
    } else {
      msh->hasTextureCoords = False;
    }
    
    if(normal) {
      poly.normal.x = *((double*)normal->data[0]->data);
      poly.normal.y = *((double*)normal->data[1]->data);
      poly.normal.z = *((double*)normal->data[2]->data);
    } else {
      msh->hasNormals = False;
    }
    
    if(tangent) {
      poly.tangent.x = *((double*)tangent->data[0]->data);
      poly.tangent.y = *((double*)tangent->data[1]->data);
      poly.tangent.z = *((double*)tangent->data[2]->data);
    } else {
      msh->hasTangents = False;
    }
    
    CogArrayPush(msh->polygons, poly);
  });

  
  // Material parsing
  //material = CogJsonObjectGetObject(json, "material");
  
  // Skeleton parsing
  //skeleton = CogJsonObjectGetObject(json, "skeleton");
  
  return msh;
}

CogJsonObject CogRawMeshToJson(const CogRawMesh self) {
  
  return NULL;
}

void CogRawMeshInspect(const CogRawMesh self) {
  printf("<CogRawMesh Object @0x%x {\n", (unsigned int)self);
  printf("  name: \"%s\"\n", self->name->data);
  printf("  polygons: "); CogArrayInspect(CogPolygon, self->polygons);
  printf("  vertices: "); CogArrayInspect(CogVertex, self->vertices);
  printf("  texCoords: "); CogArrayInspect(CogVector2, self->texCoords);
  //printf("  skeleton: "); SkeletonInspect(self->skeleton);
  printf(">\n");
}

void CogRawMeshFree(CogRawMesh self) {
  CogStringFree(self->name);
  CogArrayFree(self->texCoords);
  CogArrayFree(self->vertices);
  CogArrayFree(self->polygons);
  free(self);
}

void CogRawMeshRecalculateNormals(CogRawMesh self) {
  /* iterate over all polygons and calculate each CogPolygon normal */
  int i;
  CogVector3 v0, v1;
  for(i = 0; i < self->polygons->size; ++i) {
    v0 = CogVector3Sub(self->vertices->data[self->polygons->data[i].vertices[1]].position, self->vertices->data[self->polygons->data[i].vertices[0]].position);
    v1 = CogVector3Sub(self->vertices->data[self->polygons->data[i].vertices[2]].position, self->vertices->data[self->polygons->data[i].vertices[0]].position);
    self->polygons->data[i].normal = CogVector3Normalized(CogVector3Cross(v0, v1));
  };
  
  /* iterate over all vertices and calculate averaged normal of polygons containing that vertex */
  CogVector3 accum;
  for(i = 0; i < self->vertices->size; ++i) {
    accum = (CogVector3){0.0f, 0.0f, 0.0f};
    CogArrayForEach(self->polygons, p, {
      if(p.vertices[0] == i || p.vertices[1] == i || p.vertices[2] == i) {
        accum = CogVector3Add(accum, p.normal);
      }
    });
    self->vertices->data[i].normal = CogVector3Normalized(accum);
  }
  self->hasNormals = True;
}

void CogRawMeshRecalculateTangents(CogRawMesh self) {
  /* iterate over all polygons and calculate each polygon tangent */
  CogVector3 accum;
  int i;
  for(i = 0; i < self->polygons->size; ++i) {
    
    CogVector3 v0 = self->vertices->data[self->polygons->data[i].vertices[0]].position;
    CogVector3 v1 = self->vertices->data[self->polygons->data[i].vertices[1]].position;
    CogVector3 v2 = self->vertices->data[self->polygons->data[i].vertices[2]].position;
    
    CogVector2 uv0 = self->texCoords->data[self->polygons->data[i].texCoords[0]];
    CogVector2 uv1 = self->texCoords->data[self->polygons->data[i].texCoords[1]];
    CogVector2 uv2 = self->texCoords->data[self->polygons->data[i].texCoords[2]];
    
    CogVector3 e1 = CogVector3Sub(v1, v0);
    CogVector3 e2 = CogVector3Sub(v2, v0);
    
    float deltaU1 = uv1.x - uv0.x;
    float deltaV1 = uv1.y - uv0.y;
    float deltaU2 = uv2.x - uv0.x;
    float deltaV2 = uv2.y - uv0.y;
    
    float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);
    
    accum.x = f * (deltaV2 * e1.x - deltaV1 * e2.x);
    accum.y = f * (deltaV2 * e1.y - deltaV1 * e2.y);
    accum.z = f * (deltaV2 * e1.z - deltaV1 * e2.z);
    
    self->polygons->data[i].tangent = CogVector3Normalized(accum);
  }
  
  /* iterate over all vertices and calculate averaged tangent of polygons containing that vertex */
  for(i = 0; i < self->vertices->size; ++i) {
    accum = (CogVector3){0.0f, 0.0f, 0.0f};
    CogArrayForEach(self->polygons, p, {
      if(p.vertices[0] == i || p.vertices[1] == i || p.vertices[2] == i) {
        accum = CogVector3Add(accum, p.tangent);
      }
    });
    self->vertices->data[i].tangent = CogVector3Normalized(accum);
  }
  self->hasTangents = True;
}