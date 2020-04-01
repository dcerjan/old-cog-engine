#ifndef _COG_TEXTURE_H_
#define _COG_TEXTURE_H_

#include "cogstring.h"
#include "cogtypes.h"
#include "coggl.h"
#include "cogimageformat.h"

struct CogBundle;
struct CogTexture {
  CogString                     name;
  uint32                        width;
  uint32                        height;
  uint32                        depth;
  CogColorFormat                format;
  CogColorInternalFormat        internalFormat;
  CogTextureType                type;
  CogTextureFiltering           min;
  CogTextureFiltering           mag;
  Bool                          anisotropicFiltering;
  GLfloat                       anisotropyLevel;
  Bool                          mipmaps;
  GLuint                        id;
};
typedef struct CogTexture* CogTexture;
CogDefineHash(CogTexture);

CogTexture CogTextureAlloc(const char* name, struct CogBundle* bundle);
CogTexture CogTextureAllocFromFile(const char* name, const char* file, struct CogBundle* bundle);
void CogTextureInspect(const CogTexture self);
void CogTextureFree(CogTexture self);
void CogTextureGenerateMipmaps(const CogTexture self);
CogComponentType CogTextureGetComponentType(const CogTexture self);

#endif