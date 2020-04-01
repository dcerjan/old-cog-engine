#include "cogmesh.h"
#include "coggl.h"
#include "cogbundle.h"

CogMesh CogMeshAlloc(const char* name, struct CogBundle* bundle) {
  CogMesh self = malloc(sizeof(struct CogMesh));
  
  self->buffers = CogArrayAlloc(CogMeshBuffer);
  self->drawMethod = CogMeshDrawMethodTriangles;
  self->drawUsage = CogMeshUsageStaticDraw;
  self->name = CogStringAlloc(name);
  
  glGenVertexArrays(1, &self->id);
  
  if(bundle) {
    CogBundleAddCogMesh(bundle, self);
  }
  
  return self;
}

CogMesh CogMeshAllocPrimitiveQuad(const char* name, const float width, const float height, struct CogBundle* CogBundle) {
  CogMesh self = CogMeshAlloc(name, CogBundle);
  
  self->drawMethod = CogMeshDrawMethodTriangleFan;
  self->drawUsage = CogMeshUsageStaticDraw;
  
  // position
  CogMeshAddBuffer(self, Component3D);
  // uv
  CogMeshAddBuffer(self, Component2D);
  // normal
  CogMeshAddBuffer(self, Component3D);
  // tangent
  /*
  CogMeshAddBuffer(self, Component3D);
  */
  
  CogMeshAddComponent3(self, 0, (CogVector3){-width * 0.5f,-height * 0.5f, 0.0f});
  CogMeshAddComponent3(self, 0, (CogVector3){ width * 0.5f,-height * 0.5f, 0.0f});
  CogMeshAddComponent3(self, 0, (CogVector3){ width * 0.5f, height * 0.5f, 0.0f});
  CogMeshAddComponent3(self, 0, (CogVector3){-width * 0.5f, height * 0.5f, 0.0f});
  
  CogMeshAddComponent2(self, 1, (CogVector2){0.0f, 1.0f});
  CogMeshAddComponent2(self, 1, (CogVector2){1.0f, 1.0f});
  CogMeshAddComponent2(self, 1, (CogVector2){1.0f, 0.0f});
  CogMeshAddComponent2(self, 1, (CogVector2){0.0f, 0.0f});
  
  CogMeshAddComponent3(self, 2, (CogVector3){0.0f, 0.0f, 1.0f});
  CogMeshAddComponent3(self, 2, (CogVector3){0.0f, 0.0f, 1.0f});
  CogMeshAddComponent3(self, 2, (CogVector3){0.0f, 0.0f, 1.0f});
  CogMeshAddComponent3(self, 2, (CogVector3){0.0f, 0.0f, 1.0f});
  
  /*
  CogMeshAddComponent3(self, 3, (CogVector3){0.0f, 0.0f, 0.0f});
  CogMeshAddComponent3(self, 3, (CogVector3){0.0f, 0.0f, 0.0f});
  CogMeshAddComponent3(self, 3, (CogVector3){0.0f, 0.0f, 0.0f});
  CogMeshAddComponent3(self, 3, (CogVector3){0.0f, 0.0f, 0.0f});
  */
  
  CogMeshCompile(self);
  
  return self;
}

CogMesh CogMeshAllocPrimitiveTetrahedron(const char* name, const float width, struct CogBundle* CogBundle) {
  return NULL;
}

CogMesh CogMeshAllocPrimitiveBox(const char* name, const float width, const float height, const float depth, struct CogBundle* CogBundle) {
  return NULL;
}

CogMesh CogMeshAllocPrimitiveSphere(const char* name, const float radius, const int rings, const int meridians, struct CogBundle* CogBundle) {
  return NULL;
}

CogMesh CogMeshAllocPrimitiveCylinder(const char* name, const float radius, const float height, struct CogBundle* CogBundle) {
  return NULL;
}

CogMesh CogMeshAllocPrimitiveCone(const char* name, const float radius, const float height, struct CogBundle* CogBundle) {
  return NULL;
}

CogMesh CogMeshAllocPrimitiveCapsule(const char* name, const float radius, const float height, struct CogBundle* CogBundle) {
  return NULL;
}

void CogMeshInspect(const CogMesh self) {
  
}

void CogMeshFree(CogMesh self) {
  CogArrayForEach(self->buffers, buff, {
    CogArrayFree(buff->data);
    if(buff->id != -1) {
      glDeleteBuffers(1, &buff->id);
    }
  });
  if(self->id != -1) {
    glDeleteVertexArrays(1, &self->id);
  }
  CogStringFree(self->name);
  CogArrayFree(self->buffers);
  free(self);
}

void CogMeshCompile(CogMesh self) {
  if(self->id != -1) {
    glDeleteVertexArrays(1, &self->id);
  }
  glGenVertexArrays(1, &self->id);
  glBindVertexArray(self->id);
  int i;
  for(i = 0; i < self->buffers->size; ++i) {
    if(self->buffers->data[i]->id != -1) {
      glDeleteBuffers(1, &self->buffers->data[i]->id);
    }
    glGenBuffers(1, &self->buffers->data[i]->id);
    glBindBuffer(GL_ARRAY_BUFFER, self->buffers->data[i]->id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * self->buffers->data[i]->data->size, self->buffers->data[i]->data->data, self->drawUsage);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, self->buffers->data[i]->componentSize, GL_FLOAT, GL_FALSE, 0, 0);
  }
  self->numElements = self->buffers->data[0]->data->size / self->buffers->data[0]->componentSize;
  glBindVertexArray(0);
}

void CogMeshCompileFromRaw(CogMesh self, const CogRawMesh rawCogMesh) {
  
  // clean buffers before compilation
  CogArrayForEach(self->buffers, buff, {
    CogArrayFree(buff->data);
    if(buff->id != -1) {
      glDeleteBuffers(1, &buff->id);
    }
  });
  
  // build buffers from raw mesh
  // if any components sre missing, all subsequent buffers after the first missing will be ignored
  // buffer 0 -> position vec3
  // buffer 1 -> texCoord vec2
  // buffer 2 -> normal   vec3
  // buffer 3 -> tangent  vec3
  // buffer 4 -> bones    vec4
  // buffer 5 -> weights  vec4
  CogMeshAddBuffer(self, Component3D);         // position
  if(rawCogMesh->hasTextureCoords) {
    CogMeshAddBuffer(self, Component2D);       // texCoord
    if(rawCogMesh->hasNormals) {
      CogMeshAddBuffer(self, Component3D);     // normal
      if(rawCogMesh->hasTangents) {
        CogMeshAddBuffer(self, Component3D);   // tangent
        if(rawCogMesh->hasBones) {
          CogMeshAddBuffer(self, Component4D); // bone indices
          CogMeshAddBuffer(self, Component4D); // bone wights
        }
      }
    }
  }
  
  CogArrayForEach(rawCogMesh->polygons, p, {
    // position
    CogMeshAddComponent3(self, 0, rawCogMesh->vertices->data[p.vertices[0]].position);
    CogMeshAddComponent3(self, 0, rawCogMesh->vertices->data[p.vertices[1]].position);
    CogMeshAddComponent3(self, 0, rawCogMesh->vertices->data[p.vertices[2]].position);
    
    if(rawCogMesh->hasTextureCoords) {
      CogMeshAddComponent2(self, 1, rawCogMesh->texCoords->data[p.texCoords[0]]);
      CogMeshAddComponent2(self, 1, rawCogMesh->texCoords->data[p.texCoords[1]]);
      CogMeshAddComponent2(self, 1, rawCogMesh->texCoords->data[p.texCoords[2]]);
      
      if(rawCogMesh->hasNormals) {
        CogMeshAddComponent3(self, 2, rawCogMesh->vertices->data[p.vertices[0]].normal);
        CogMeshAddComponent3(self, 2, rawCogMesh->vertices->data[p.vertices[1]].normal);
        CogMeshAddComponent3(self, 2, rawCogMesh->vertices->data[p.vertices[2]].normal);
      
        if(rawCogMesh->hasTangents) {
          CogMeshAddComponent3(self, 3, rawCogMesh->vertices->data[p.vertices[0]].tangent);
          CogMeshAddComponent3(self, 3, rawCogMesh->vertices->data[p.vertices[1]].tangent);
          CogMeshAddComponent3(self, 3, rawCogMesh->vertices->data[p.vertices[2]].tangent);
          
          if(rawCogMesh->hasBones) {
            CogVector4 b0;
            CogVector4 b1;
            CogVector4 b2;
            
            b0.x = rawCogMesh->vertices->data[p.vertices[0]].bone[0];
            b0.y = rawCogMesh->vertices->data[p.vertices[0]].bone[1];
            b0.z = rawCogMesh->vertices->data[p.vertices[0]].bone[2];
            b0.w = rawCogMesh->vertices->data[p.vertices[0]].bone[3];
            
            b1.x = rawCogMesh->vertices->data[p.vertices[1]].bone[0];
            b1.y = rawCogMesh->vertices->data[p.vertices[1]].bone[1];
            b1.z = rawCogMesh->vertices->data[p.vertices[1]].bone[2];
            b1.w = rawCogMesh->vertices->data[p.vertices[1]].bone[3];
            
            b2.x = rawCogMesh->vertices->data[p.vertices[2]].bone[0];
            b2.y = rawCogMesh->vertices->data[p.vertices[2]].bone[1];
            b2.z = rawCogMesh->vertices->data[p.vertices[2]].bone[2];
            b2.w = rawCogMesh->vertices->data[p.vertices[2]].bone[3];
            
            CogMeshAddComponent4(self, 4, b0);
            CogMeshAddComponent4(self, 4, b1);
            CogMeshAddComponent4(self, 4, b2);
            
            CogVector4 w0;
            CogVector4 w1;
            CogVector4 w2;
            
            w0.x = rawCogMesh->vertices->data[p.vertices[0]].weight[0];
            w0.y = rawCogMesh->vertices->data[p.vertices[0]].weight[1];
            w0.z = rawCogMesh->vertices->data[p.vertices[0]].weight[2];
            w0.w = rawCogMesh->vertices->data[p.vertices[0]].weight[3];
            
            w1.x = rawCogMesh->vertices->data[p.vertices[1]].weight[0];
            w1.y = rawCogMesh->vertices->data[p.vertices[1]].weight[1];
            w1.z = rawCogMesh->vertices->data[p.vertices[1]].weight[2];
            w1.w = rawCogMesh->vertices->data[p.vertices[1]].weight[3];
            
            w2.x = rawCogMesh->vertices->data[p.vertices[2]].weight[0];
            w2.y = rawCogMesh->vertices->data[p.vertices[2]].weight[1];
            w2.z = rawCogMesh->vertices->data[p.vertices[2]].weight[2];
            w2.w = rawCogMesh->vertices->data[p.vertices[2]].weight[3];
            
            CogMeshAddComponent4(self, 5, w0);
            CogMeshAddComponent4(self, 5, w1);
            CogMeshAddComponent4(self, 5, w2);
          }
        }
      }
    }
    
  });
  
  
  if(self->id != -1) {
    glDeleteVertexArrays(1, &self->id);
  }
  glGenVertexArrays(1, &self->id);
  glBindVertexArray(self->id);
  int i;
  for(i = 0; i < self->buffers->size; ++i) {
    if(self->buffers->data[i]->id != -1) {
      glDeleteBuffers(1, &self->buffers->data[i]->id);
    }
    glGenBuffers(1, &self->buffers->data[i]->id);
    glBindBuffer(GL_ARRAY_BUFFER, self->buffers->data[i]->id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * self->buffers->data[i]->data->size, self->buffers->data[i]->data->data, self->drawUsage);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, self->buffers->data[i]->componentSize, GL_FLOAT, GL_FALSE, 0, 0);
  }
  self->numElements = self->buffers->data[0]->data->size / self->buffers->data[0]->componentSize;
  glBindVertexArray(0);
}

void CogMeshAddBuffer(CogMesh self, const ComponentSize componentSize) {
  CogMeshBuffer buffer = (CogMeshBuffer)malloc(sizeof(struct CogMeshBuffer));
  buffer->componentSize = componentSize;
  buffer->data = CogArrayAlloc(float);
  buffer->id = -1;
  CogArrayPush(self->buffers, buffer);
}

void CogMeshAddComponent1(CogMesh self, const int buffer, const float component) {
  if(buffer < self->buffers->size) {
    if(self->buffers->data[buffer]->componentSize == Component1D) {
      CogArrayPush(self->buffers->data[buffer]->data, component);
    } else {
      printf("[Error] Wrong component size! Have 1, buffer expects %d.\n", self->buffers->data[buffer]->componentSize);
      exit(-1);
    }
  } else {
    printf("[Error] Buffer index out of bounds: %d of %d.\n", buffer, self->buffers->size);
    exit(-1);
  }
}

void CogMeshAddComponent2(CogMesh self, const int buffer, const CogVector2 component) {
  if(buffer < self->buffers->size) {
    if(self->buffers->data[buffer]->componentSize == Component2D) {
      CogArrayPush(self->buffers->data[buffer]->data, component.x);
      CogArrayPush(self->buffers->data[buffer]->data, component.y);
    } else {
      printf("[Error] Wrong component size! Have 2, buffer expects %d.\n", self->buffers->data[buffer]->componentSize);
      exit(-1);
    }
  } else {
    printf("[Error] Buffer index out of bounds: %d of %d.\n", buffer, self->buffers->size);
    exit(-1);
  }
}

void CogMeshAddComponent3(CogMesh self, const int buffer, const CogVector3 component) {
  if(buffer < self->buffers->size) {
    if(self->buffers->data[buffer]->componentSize == Component3D) {
      CogArrayPush(self->buffers->data[buffer]->data, component.x);
      CogArrayPush(self->buffers->data[buffer]->data, component.y);
      CogArrayPush(self->buffers->data[buffer]->data, component.z);
    } else {
      printf("[Error] Wrong component size! Have 3, buffer expects %d.\n", self->buffers->data[buffer]->componentSize);
      exit(-1);
    }
  } else {
    printf("[Error] Buffer index out of bounds: %d of %d.\n", buffer, self->buffers->size);
    exit(-1);
  }
}

void CogMeshAddComponent4(CogMesh self, const int buffer, const CogVector4 component) {
  if(buffer < self->buffers->size) {
    if(self->buffers->data[buffer]->componentSize == Component4D) {
      CogArrayPush(self->buffers->data[buffer]->data, component.x);
      CogArrayPush(self->buffers->data[buffer]->data, component.y);
      CogArrayPush(self->buffers->data[buffer]->data, component.z);
      CogArrayPush(self->buffers->data[buffer]->data, component.w);
    } else {
      printf("[Error] Wrong component size! Have 4, buffer expects %d.\n", self->buffers->data[buffer]->componentSize);
      exit(-1);
    }
  } else {
    printf("[Error] Buffer index out of bounds: %d of %d.\n", buffer, self->buffers->size);
    exit(-1);
  }
}

