#ifndef _COG_RESOURCE_MANAGER_H_
#define _COG_RESOURCE_MANAGER_H_

#include "cogstring.h"
#include "CogHash.h"
#include "cogbundle.h"

struct CogResourceManager {
  CogHash(CogBundle)  bundles;
};
typedef struct CogResourceManager* CogResourceManager;

void
CogResourceManagerInit(
  void
);

void
CogResourceManagerCleanup(
  void
);

CogResourceManager
CogResourceManagerGet(
  void
);

CogBundle
CogResourceManagerAddBundle(
  CogBundle CogBundle
);

void
CogResourceManagerLoadBundle(
  const char* path
);

CogBundle
CogResourceManagerGetBundle(
  const char* name
);

void
CogResourceManagerRemoveBundle(
  const char* name
);


#endif