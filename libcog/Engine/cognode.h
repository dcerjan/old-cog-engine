#ifndef _COG_NODE_H_
#define _COG_NODE_H_

#include "CogVector3.h"
#include "cogmatrix4.h"
#include "cogstring.h"

typedef enum CogNodeSpace {
  CogNodeSpaceWorld,
  CogNodeSpaceLocal,
  CogNodeSpaceCogCamera
} CogNodeSpace;

// forward declarations
struct CogModel;
struct CogLight;
struct CogCamera;
struct CogParticleEmitter;
struct CogShader;

struct CogList_Model;
struct CogList_Light;
struct CogList_CogCamera;
struct CogList_ParticleEmitter;

struct CogList_CogNode;
struct CogNode {
  CogString             name;
  
  CogMatrix4            rotation;
  CogVector3            position;
  CogVector3            scale;
  
  CogMatrix4            model;
  
  Bool                  dirty;
  
  struct {
    struct CogList_CogModel*              models;
    struct CogList_CogLight*              lights;
    struct CogList_CogCamera*             cameras;
    struct CogList_CogParticleEmitter*    emitters;
  } attachments;
  
  struct CogNode*       parent;
  struct CogList_CogNode*  children;
};
typedef struct CogNode* CogNode;
CogDefineList(CogNode);

CogNode   CogNodeAlloc              (const char* name, CogNode parent);
void      CogNodeInspect            (CogNode self);
void      CogNodeFree               (CogNode self);

// entity attachment
void      CogNodeAttachModel        (CogNode self, struct CogModel* model);
void      CogNodeAttachLight        (CogNode self, struct CogLight* light);
void      CogNodeAttachCamera       (CogNode self, struct CogCamera* camera);
void      CogNodeAttachEmitter      (CogNode self, struct CogParticleEmitter* emitter);

// node attachment stuff
CogNode   CogNodeGetChildNode       (CogNode self, const char* name);
CogNode   CogNodeAttachNode         (CogNode self, CogNode child);
CogNode   CogNodeDetachNode         (CogNode self, CogNode child);

// all transform functions are returning modified self to allow chaining
CogNode   CogNodeSetRotatation      (CogNode self, CogVector3 axis, float angle);
CogNode   CogNodeSetTranslation     (CogNode self, CogVector3 position);
CogNode   CogNodeSetScale           (CogNode self, float s, float t, float u);
CogNode   CogNodeTranslate          (CogNode self, CogVector3 position);
CogNode   CogNodeTranslateLeft      (CogNode self, float t);
CogNode   CogNodeTranslateRight     (CogNode self, float t);
CogNode   CogNodeTranslateUp        (CogNode self, float t);
CogNode   CogNodeTranslateDown      (CogNode self, float t);
CogNode   CogNodeTranslateForward   (CogNode self, float t);
CogNode   CogNodeTranslateBackward  (CogNode self, float t);
CogNode   CogNodeRotate             (CogNode self, CogVector3 axis, float angle);
CogNode   CogNodeYaw                (CogNode self, float angle);
CogNode   CogNodePitch              (CogNode self, float angle);
CogNode   CogNodeRoll               (CogNode self, float angle);
CogNode   CogNodeScale              (CogNode self, float s, float t, float u);
CogNode   CogNodeLookAt             (CogNode self, CogVector3 target, CogVector3 up);

// getters
CogVector3 CogNodeGetPosition       (CogNode self, const CogNodeSpace space);
CogVector3 CogNodeGetScale          (CogNode self);

// recursive update
void    CogNodeUpdateTransforms     (CogNode self);
void    CogNodeUpdate               (CogNode self, float t);

#endif