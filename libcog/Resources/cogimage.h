#ifndef _COG_IMAGE_H_
#define _COG_IMAGE_H_

#include "cogtypes.h"
#include "cogstring.h"
#include "cogimageformat.h"

struct Texture;
struct CogBundle;
struct CogImage {
  CogString      name;
  CogColorFormat format;
  ubyte*      data;
  int         width;
  int         height;
  int         bpp;
};
typedef struct CogImage* CogImage;
CogDefineArray(CogImage);
CogDefineList(CogImage);
CogDefineHash(CogImage);

CogImage           CogImageAlloc        (const char* name, const int width, const int height, const CogColorFormat format, struct CogBundle* CogBundle);
CogImage           CogImageAllocFromFile(const char* name, const char* file, struct CogBundle* CogBundle);
void            CogImageClear        (CogImage self);
void            CogImageResize       (CogImage self, const int width, const int height);
void            CogImageSetFormat    (CogImage self, const CogColorFormat format);
void            CogImageInspect      (const CogImage self);
void            CogImageFree         (CogImage self);
struct Texture* CogImageCreateTexture(CogImage self, struct CogBundle* CogBundle);

#endif