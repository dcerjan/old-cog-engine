#include "cogimage.h"
#include "cogbundle.h"
#include "cogtexture.h"
#include "coglogger.h"
#include "cogplatform.h"
#include <IL/il.h>

CogImage CogImageAlloc(const char* name, const int width, const int height, const CogColorFormat format, struct CogBundle* bundle) {
  int sz;
  switch(format) {
    case CogColorFormatRED: sz = 1; break;
    case CogColorFormatRG: sz = 2; break;
    case CogColorFormatRGB: sz = 3; break;
    case CogColorFormatRGBA: sz = 4; break;
    default: sz = 1;
  }
  
  CogImage self = malloc(sizeof(struct CogImage));
  self->name = CogStringAlloc(name);
  self->width = width >= 1 ? width : 1;
  self->height = height >= 1 ? height : 1;
  self->data = malloc(sz * width * height);
  self->bpp = sz * 8;
  memset(self->data, 0, sz * width * height);
  
  if(bundle) {
    CogBundleAddImage(bundle, self);
  }
  
  return self;
}
CogImage CogImageAllocFromFile(const char* name, const char* file, struct CogBundle* bundle) {
  ILuint img = ilGenImage();
  ilBindImage(img);
  if(IL_FALSE == ilLoadImage(file)) {
    CogLoggerError(CogPlatformGetLogger(), "Unable to open file %s.\n", file);
  } else if(IL_FALSE == ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
    CogLoggerError(CogPlatformGetLogger(), "Unable to convert file %s to RGBA8 color format.\n", file);
  }
  
  CogImage self = CogImageAlloc(name, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), CogColorFormatRGBA, bundle);
  self->format = ilGetInteger(IL_IMAGE_FORMAT);
  self->bpp = ilGetInteger(IL_IMAGE_BPP);
  memcpy(self->data, ilGetData(), self->height * self->width * 4);
  ilBindImage(0);
  ilDeleteImage(img);
  return self;
}

void CogImageClear(CogImage self) {
  size_t sz;
  switch(self->format) {
    case CogColorFormatRED: sz = 1; break;
    case CogColorFormatRG: sz = 2; break;
    case CogColorFormatRGB: sz = 3; break;
    case CogColorFormatRGBA: sz = 4; break;
    default: sz = 1;
  }
  self->width = self->width >= 1 ? self->width : 1;
  self->height = self->height >= 1 ? self->height : 1;
  memset(self->data, 0, sz * self->width * self->height);
}

void CogImageResize(CogImage self, const int width, const int height) {
  size_t sz;
  switch(self->format) {
    case CogColorFormatRED: sz = 1; break;
    case CogColorFormatRG: sz = 2; break;
    case CogColorFormatRGB: sz = 3; break;
    case CogColorFormatRGBA: sz = 4; break;
    default: sz = 1;
  }
  self->width = width >= 1 ? width : 1;
  self->height = height >= 1 ? height : 1;
  memset(self->data, 0, sz * self->width * self->height);
}

void CogImageSetFormat(CogImage self, const CogColorFormat format) {
  int sz;
  self->format = format;
  switch(format) {
    case CogColorFormatRED: sz = 1; break;
    case CogColorFormatRG: sz = 2; break;
    case CogColorFormatRGB: sz = 3; break;
    case CogColorFormatRGBA: sz = 4; break;
    default: sz = 1;
  }
  self->bpp = sz * 8;
  self->width = self->width >= 1 ? self->width : 1;
  self->height = self->height >= 1 ? self->height : 1;
  memset(self->data, 0, sz * self->width * self->height);
}

void CogImageInspect(const CogImage self) {
  size_t sz;
  switch(self->format) {
    case CogColorFormatRED: sz = 1; break;
    case CogColorFormatRG: sz = 2; break;
    case CogColorFormatRGB: sz = 3; break;
    case CogColorFormatRGBA: sz = 4; break;
    default: sz = 1;
  }
  printf("<CogImage @%xu %d x %d x %lu>\n", (unsigned int)self, self->width, self->height, sz);
}

void CogImageFree(CogImage self) {
  CogStringFree(self->name);
  free(self->data);
  free(self);
}

struct Texture* CogImageCreateTexture(CogImage self, struct CogBundle* CogBundle) {
  CogTexture tex = CogTextureAlloc(self->name->data, CogBundle);
  
  tex->format = CogColorFormatRGBA;
  tex->internalFormat = CogColorInternalFormatRGBA8;
  
  glBindTexture(GL_TEXTURE_2D, tex->id);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
  tex->width = self->width;
  tex->height = self->height;
  
  glTexImage2D(GL_TEXTURE_2D, 0, tex->internalFormat, self->width, self->height, 0, self->format, GL_UNSIGNED_BYTE, self->data);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  return tex;
}