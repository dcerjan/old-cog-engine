#include "coggc.h"

CogCleanupPool CogCleanupPoolAlloc(const char* name) {
  CogCleanupPool self = malloc(sizeof(struct CogCleanupPool));
  self->name = CogStringAlloc(name);
  self->cleanupPool = CogListAlloc(CogGCEntry);
  return self;
}

void CogCleanupPoolFree(CogCleanupPool self) {
  CogListForEach(self->cleanupPool, ent, {
    ent.destructor(ent.object);
  });
  CogListFree(self->cleanupPool);
  CogStringFree(self->name);
  free(self);
}

void CogCleanupPoolRegisterForCleanup(CogCleanupPool self, void* object, auto_destruct destructor) {
  CogGCEntry e = {.object = object, .destructor = destructor};
  CogListPushBack(self->cleanupPool, e);
}