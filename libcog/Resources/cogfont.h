#ifndef _COG_FONT_H_
#define _COG_FONT_H_

#include "cogstring.h"
#include "CogHash.h"
#include "cogimage.h"

struct CogFont {
  CogString name;
  // TODO
};
typedef struct CogFont* CogFont;
CogDefineHash(CogFont)

CogFont CogFontAlloc(const char* name, const float size, struct CogBundle* bundle);
void CogFontInspect(const CogFont self);
void CogFontFree(CogFont self);

#endif