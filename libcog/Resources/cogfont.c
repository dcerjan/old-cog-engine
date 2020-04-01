#include "cogfont.h"
#include "cogbundle.h"

CogFont CogFontAlloc(const char* name, const float size, struct CogBundle* bundle) {
  CogFont self = malloc(sizeof(struct CogFont));
  
  self->name = CogStringAlloc(name);
  
  if(bundle) {
    CogBundleAddFont(bundle, self);
  }
  
  return self;
}

void CogFontInspect(const CogFont self) {
  
}

void CogFontFree(CogFont self) {
  
}