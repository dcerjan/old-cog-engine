#ifndef _COG_SCENEGRAPH_H_
#define _COG_SCENEGRAPH_H_


struct CogNode;
struct CogList_CogNode;
struct CogSceneGraph {
  struct CogNode*         root;
  struct CogList_CogNode* nodes;
};
typedef struct CogSceneGraph* CogSceneGraph;

CogSceneGraph
CogSceneGraphAlloc(
  void
);

void
CogSceneGraphInspect(
  const CogSceneGraph self
);

void
CogSceneGraphFree(
  CogSceneGraph self
);

void
CogSceneGraphUpdate(
  CogSceneGraph self, 
  const double t
);


#endif