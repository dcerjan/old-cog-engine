#ifndef _COG_RENDERER_H_
#define _COG_RENDERER_H_

#include "cogstring.h"

struct CogRenderer;
struct CogShader;
struct CogMaterial;
struct CogMesh;
struct CogNode;
struct CogScene;
struct CogCamera;
struct CogLight;

typedef void (*renderScene) (struct CogRenderer*, const struct CogScene*);  
typedef void (*renderCogNode)  (struct CogRenderer*, const struct CogScene*, const struct CogShader*, const struct CogNode*);
typedef void (*bindShader)  (struct CogRenderer*, const struct CogScene*, const struct CogShader*);
typedef void (*bindMaterial)(struct CogRenderer*, const struct CogScene*, const struct CogShader*, const struct CogMaterial*);
typedef void (*bindCogCamera)  (struct CogRenderer*, const struct CogScene*, const struct CogShader*, const struct CogCamera*);
typedef void (*bindCogNode)    (struct CogRenderer*, const struct CogScene*, const struct CogShader*, const struct CogNode*);
typedef void (*bindCogMesh)    (struct CogRenderer*, const struct CogScene*, const struct CogShader*, const struct CogMesh*);
typedef void (*bindLight)   (struct CogRenderer*, const struct CogScene*, const struct CogShader*, const struct CogLight*);

struct CogRenderer {
  CogString   name;
  
  void (*renderScene) (struct CogRenderer* self, const struct CogScene* scene);  
  void (*renderCogNode)  (struct CogRenderer* self, const struct CogScene* scene, const struct CogShader* shader, const struct CogNode* node);

  void (*bindShader)  (struct CogRenderer* self, const struct CogScene* scene, const struct CogShader* shader);
  void (*bindMaterial)(struct CogRenderer* self, const struct CogScene* scene, const struct CogShader* shader, const struct CogMaterial* material);
  void (*bindCogCamera)  (struct CogRenderer* self, const struct CogScene* scene, const struct CogShader* shader, const struct CogCamera* CogCamera);
  void (*bindCogNode)    (struct CogRenderer* self, const struct CogScene* scene, const struct CogShader* shader, const struct CogNode* node);
  void (*bindCogMesh)    (struct CogRenderer* self, const struct CogScene* scene, const struct CogShader* shader, const struct CogMesh* mesh);
  void (*bindLight)   (struct CogRenderer* self, const struct CogScene* scene, const struct CogShader* shader, const struct CogLight* light);
  
  void (*free)        (struct CogRenderer* self);
  void (*inspect)     (struct CogRenderer* self);
};
typedef struct CogRenderer* CogRenderer;


#endif