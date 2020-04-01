#include "cognode.h"
#include "cogcamera.h"
#include "coglight.h"
#include "cogmodel.h"
#include "cogplatform.h"
#include "cogscene.h"
#include "cogscenemanager.h"
#include "cogshader.h"

CogNode CogNodeAlloc(const char* name, CogNode parent) {
  CogNode self = malloc(sizeof(struct CogNode));
  
  self->name = CogStringAlloc(name);
  self->children = CogListAlloc(CogNode);
  self->dirty = True;
  self->model = CogMatrix4Identity();
  self->scale = (CogVector3){1.0f, 1.0f, 1.0f};
  self->position = (CogVector3){0.0f, 0.0f, 0.0f};
  self->rotation = CogMatrix4Identity();
  self->parent = parent;
  
  if(parent) {
    CogNodeAttachNode(parent, self);
  }
  
  self->attachments.cameras = CogListAlloc(CogCamera);
  //self->attachments.emitters = CogListAlloc(CogParticleEmitter);
  self->attachments.lights = CogListAlloc(CogLight);
  self->attachments.models = CogListAlloc(CogModel);

  return self;
}

void CogNodeInspect(CogNode self) {
  
}

void CogNodeFree(CogNode self) {
  CogStringFree(self->name);
  
  CogListFree(self->attachments.cameras);
  //CogListFree(self->attachments.emitters);
  CogListFree(self->attachments.lights);
  CogListFree(self->attachments.models);
  
  free(self);
}

// entity attachment
void CogNodeAttachModel(CogNode self, struct CogModel* model) {
  CogListPushBack(self->attachments.models, model);
}

void CogNodeAttachLight(CogNode self, struct CogLight* light) {
  CogListPushBack(self->attachments.lights, light);
}
void CogNodeAttachCamera(CogNode self, struct CogCamera* CogCamera) {
  CogListPushBack(self->attachments.cameras, CogCamera);
  CogCamera->anchor = self;
}
void CogNodeAttachEmitter(CogNode self, struct CogParticleEmitter* emitter) {
  
}

// node attachment stuff
CogNode CogNodeGetChildNode(CogNode self, const char* name) {
  CogListForEach(self->children, node, {
    if(!strcmp(name, node->name->data)) {
      return node;
    }
  });
  return NULL;
}

CogNode CogNodeAttachNode(CogNode self, CogNode child) {
  CogListForEach(self->children, node, {
    if(!strcmp(c_str(child->name), c_str(node->name))) {
      CogLoggerWarning(CogPlatformGetLogger(), "[CogNode] CogNode %s already contains child named %s.\n", c_str(self->name), c_str(child->name));
      return child;
    }
  });
  CogListPushBack(self->children, child);
  return child;
}

CogNode CogNodeDetachNode(CogNode self, CogNode child) {
  return CogListRemove(self->children, child);
}

// all transform functions are returning modified self to allow chaining
CogNode CogNodeSetRotatation(CogNode self, CogVector3 axis, float angle) {
  self->dirty = True;
  self->rotation = CogMatrix4Rotation(CogVector3Normalized(axis), angle);
  return self;
}

CogNode CogNodeSetTranslation(CogNode self, CogVector3 position) {
  self->dirty = True;
  self->position = position;
  return self;
}

CogNode CogNodeSetScale(CogNode self, float s, float t, float u) {
  self->dirty = True;
  self->scale = (CogVector3){s,t,u};
  return self;
}

CogNode CogNodeTranslate(CogNode self, CogVector3 position) {
  self->dirty = True;
  self->position = CogVector3Add(self->position, position);
  return self;
}

CogNode CogNodeTranslateLeft(CogNode self, float t) {
  self->dirty = True;
  self->position = CogVector3Sub(self->position, (CogVector3){self->rotation.vector.x.x * t, self->rotation.vector.x.y * t, self->rotation.vector.x.z * t});
  return self;
}

CogNode CogNodeTranslateRight(CogNode self, float t) {
  self->dirty = True;
  self->position = CogVector3Add(self->position, (CogVector3){self->rotation.vector.x.x * t, self->rotation.vector.x.y * t, self->rotation.vector.x.z * t});
  return self;
}

CogNode CogNodeTranslateUp(CogNode self, float t) {
  self->dirty = True;
  self->position = CogVector3Add(self->position, (CogVector3){self->rotation.vector.y.x * t, self->rotation.vector.y.y * t, self->rotation.vector.y.z * t});
  return self;
}

CogNode CogNodeTranslateDown(CogNode self, float t) {
  self->dirty = True;
  self->position = CogVector3Sub(self->position, (CogVector3){self->rotation.vector.y.x * t, self->rotation.vector.y.y * t, self->rotation.vector.y.z * t});
  return self;
}

CogNode CogNodeTranslateForward(CogNode self, float t) {
  self->dirty = True;
  self->position = CogVector3Sub(self->position, (CogVector3){self->rotation.vector.z.x * t, self->rotation.vector.z.y * t, self->rotation.vector.z.z * t});
  return self;
}

CogNode CogNodeTranslateBackward(CogNode self, float t) {
  self->dirty = True;
  self->position = CogVector3Add(self->position, (CogVector3){self->rotation.vector.z.x * t, self->rotation.vector.z.y * t, self->rotation.vector.z.z * t});
  return self;
}

CogNode CogNodeRotate(CogNode self, CogVector3 axis, float angle) {
  self->dirty = True;
  self->rotation = CogMatrix4MulMatrix4(self->rotation, CogMatrix4Rotation( CogVector3Normalized(axis), angle));
  return self;
}

CogNode CogNodeYaw(CogNode self, float angle) {
  self->dirty = True;
  self->rotation = CogMatrix4MulMatrix4(CogMatrix4Rotation((CogVector3){0.0f, 1.0f, 0.0f}, angle), self->rotation);
  return self;
}

CogNode CogNodePitch(CogNode self, float angle) {
  self->dirty = True;
  self->rotation = CogMatrix4MulMatrix4(CogMatrix4Rotation((CogVector3){1.0f, 0.0f, 0.0f}, angle), self->rotation);
  return self;
}

CogNode CogNodeRoll(CogNode self, float angle) {
  self->dirty = True;
  self->rotation = CogMatrix4MulMatrix4(CogMatrix4Rotation((CogVector3){0.0f, 0.0f, -1.0f}, angle), self->rotation);
  return self;
}

CogNode CogNodeScale(CogNode self, float s, float t, float u) {
  self->dirty = True;
  self->scale.x *= s;
  self->scale.y *= t;
  self->scale.z *= u;
  return self;
}

CogNode CogNodeLookAt(CogNode self, CogVector3 target, CogVector3 up) {
  self->dirty = True;
  CogVector3 wp = {self->model.vector.w.x, self->model.vector.w.y, self->model.vector.w.z};
  self->rotation = CogMatrix4LookAt(wp, target, up);
  self->rotation.vector.w = (CogVector4){0.0f, 0.0f, 0.0f, 1.0f};
  return self;
}

CogVector3 CogNodeGetPosition(CogNode self, const CogNodeSpace space) {
  switch(space) {
    case CogNodeSpaceLocal:
      return self->position;
    case CogNodeSpaceWorld: {
      CogMatrix4 toWorld = CogMatrix4Inverted(self->model);
      return (CogVector3){-toWorld.vector.w.x, -toWorld.vector.w.y, -toWorld.vector.w.z};
    }
    case CogNodeSpaceCogCamera:
      return (CogVector3){self->model.vector.w.x, self->model.vector.w.y, self->model.vector.w.z};
  }
}

CogVector3 CogNodeGetScale(CogNode self) {
  return self->scale;
}

void CogNodeUpdateTransforms(CogNode self) {
  if(self->dirty) {
    CogMatrix4 T = CogMatrix4Translation(self->position);
    CogMatrix4 S = CogMatrix4Scale(self->scale.x, self->scale.y, self->scale.z);
    CogMatrix4 R = self->rotation;
    CogMatrix4 M = CogMatrix4MulMatrix4( CogMatrix4MulMatrix4(S, T), R );
    
    if(self->parent) {
      self->model = CogMatrix4MulMatrix4( self->parent->model, M );
    } else {
      self->model = M;
    }
  }
  
  CogListForEach(self->children, child, {
    CogNodeUpdateTransforms(child);
  });
}

void CogNodeUpdate(CogNode self, float t) {
  /* TODO: update attached models, particle emitters and CogCameras*/
  CogListForEach(self->attachments.cameras, CogCamera, {
    CogCamera->view = CogMatrix4Inverted(self->model);
  });
  
  CogListCogNode_CogLight* light;
  CogCamera c = CogSceneManagerGetActiveScene()->activeCamera;
  for(light = self->attachments.lights->first; light != NULL; light = light->next) {
    if(light->data->type == CogLightTypePoint) {
      CogPointLight l = (CogPointLight)light->data;
      l->position = CogMatrix4MulCogVector3(c->view, (CogVector3){self->model.vector.w.x, self->model.vector.w.y, self->model.vector.w.z});
    } else if(light->data->type == CogLightTypeDirectional) {
      CogDirectionalLight l = (CogDirectionalLight)light->data;
      CogVector4 camSpaceDirection = CogMatrix4MulVector4(c->view, (CogVector4){self->model.vector.z.x, self->model.vector.z.y, self->model.vector.z.z, 0.0f});
      l->direction.x = camSpaceDirection.x;
      l->direction.y = camSpaceDirection.y;
      l->direction.z = camSpaceDirection.z;
    } else if(light->data->type == CogLightTypeSpot) {
      CogSpotLight l = (CogSpotLight)light->data;
      l->position = CogMatrix4MulCogVector3(c->view, (CogVector3){self->model.vector.w.x, self->model.vector.w.y, self->model.vector.w.z});
      CogVector4 camSpaceDirection = CogMatrix4MulVector4(c->view, (CogVector4){self->model.vector.z.x, self->model.vector.z.y, self->model.vector.z.z, 0.0f});
      l->direction.x = camSpaceDirection.x;
      l->direction.y = camSpaceDirection.y;
      l->direction.z = camSpaceDirection.z;
    }
  }
  CogListForEach(self->children, child, {
    CogNodeUpdate(child, t);
  });
}

void CogNodeBind(CogNode self, struct CogShader* shader) {
  glUniformMatrix4fv(shader->uniforms.vertex.matrix.model, 1, GL_FALSE, (const float*)&self->model);
}