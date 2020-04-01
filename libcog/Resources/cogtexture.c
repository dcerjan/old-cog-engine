#include "cogtexture.h"
#include "cogbundle.h"
#include "cogimage.h"
#include "coglogger.h"
#include "cogplatform.h"

/***********/
/* helpers */

static CogTextureFiltering __cog_tfCogArray__[] = {CogTextureFilteringNearest, CogTextureFilteringLinear, CogTextureFilteringNearestMipmapNearest, CogTextureFilteringNearestMipmapLinear, CogTextureFilteringLinearMipmapNearest, CogTextureFilteringLinearMipmapLinear};
static const char* __cog_tfnameCogArray__[] = {"Nearest Filtering", "Linear Filtering", "Nearest Mipmap Nearest Filtering", 
                            "Nearest Mipmap Linear Filtering", "Linear Mipmap Nearest Filtering", "Linear Mipmap Linear Filtering"};
                            
static CogTextureType __cog_ttCogArray__[] = {CogTextureTypeTexture1D, CogTextureTypeTexture2D, CogTextureTypeTexture3D, CogTextureTypeTextureCubeMap};
static const char* __cog_ttnameCogArray__[] = {"1D Texture", "2D Texture", "3D Texture", "Cubemap Texture"};


static CogColorFormat __cog_cfCogArray__[] = {CogColorFormatRGBA, CogColorFormatRGB, CogColorFormatRG, CogColorFormatRED};
static const char* __cog_cfnameCogArray__[] = {"RGBA", "RGB", "RG", "RED"};

static CogColorInternalFormat __cog_cifCogArray__[] = {
  CogColorInternalFormatRGBA8, CogColorInternalFormatRGB8, CogColorInternalFormatRG8, CogColorInternalFormatR8,
#ifdef GL_RGBA16UI
  CogColorInternalFormatRGBA16UI, CogColorInternalFormatRGB16UI, CogColorInternalFormatRG16UI, CogColorInternalFormatR16UI,
#endif
#ifdef GL_RGBA32UI
  CogColorInternalFormatRGBA32UI, CogColorInternalFormatRGB32UI, CogColorInternalFormatRG32UI, CogColorInternalFormatR32UI,
#endif
#ifdef GL_RGBA16I
  CogColorInternalFormatRGBA16I, CogColorInternalFormatRGB16I, CogColorInternalFormatRG16I, CogColorInternalFormatR16I,
#endif
#ifdef GL_RGBA32I
  CogColorInternalFormatRGBA32I, CogColorInternalFormatRGB32I, CogColorInternalFormatRG32I, CogColorInternalFormatR32I,
#endif
#ifdef GL_RGBA16F
  CogColorInternalFormatRGBA16F, CogColorInternalFormatRGB16F, CogColorInternalFormatRG16F, CogColorInternalFormatR16F,
#endif
#ifdef GL_RGBA32F
  CogColorInternalFormatRGBA32F, CogColorInternalFormatRGB32F, CogColorInternalFormatRG32F, CogColorInternalFormatR32F,
#endif
  0
};

static CogComponentType __cog_comtarray__[] = {
  CogComponentTypeUByte, CogComponentTypeUByte, CogComponentTypeUByte, CogComponentTypeUByte,
#ifdef GL_RGBA16UI
  CogComponentTypeUShort, CogComponentTypeUShort, CogComponentTypeUShort, CogComponentTypeUShort,
#endif
#ifdef GL_RGBA32UI
  CogComponentTypeUInt, CogComponentTypeUInt, CogComponentTypeUInt, CogComponentTypeUInt,
#endif
#ifdef GL_RGBA16I
  CogComponentTypeShort, CogComponentTypeShort, CogComponentTypeShort, CogComponentTypeShort,
#endif
#ifdef GL_RGBA32I
  CogComponentTypeInt, CogComponentTypeInt, CogComponentTypeInt, CogComponentTypeInt,
#endif
#ifdef GL_RGBA16F
  CogComponentTypeHalf, CogComponentTypeHalf, CogComponentTypeHalf, CogComponentTypeHalf,
#endif
#ifdef GL_RGBA32F
  CogComponentTypeFloat, CogComponentTypeFloat, CogComponentTypeFloat, CogComponentTypeFloat,
#endif
  0
};
static const char* __cog_cifnameCogArray__[] = {"4 channel unsigned byte(8)", "3 channel unsigned byte(8)", "2 channel unsigned byte(8)", "1 channel unsigned byte(8)",
#ifdef GL_RGBA16UI
  "4 channel unsigned short(16)", "3 channel unsigned short(16)", "2 channel unsigned short(16)", "1 channel unsigned short(16)",
#endif
#ifdef GL_RGBA32UI
  "4 channel unsigned int(32)", "3 channel unsigned int(32)", "2 channel unsigned int(32)", "1 channel unsigned int(32)",
#endif
#ifdef GL_RGBA16I
  "4 channel short(16)", "3 channel short(16)", "2 channel short(16)", "1 channel short(16)",
#endif
#ifdef GL_RGBA32I
  "4 channel int(32)", "3 channel int(32)", "2 channel int(32)", "1 channel int(32)",
#endif
#ifdef GL_RGBA16F
  "4 channel half(16)", "3 channel half(16)", "2 channel half(16)", "1 channel half(16)",
#endif
#ifdef GL_RGBA32F
  "4 channel float(32)", "3 channel float(32)", "2 channel float(32)", "1 channel float(32)",
#endif
  "0"
};
static size_t __cog_szCogArray__[] = {32, 24, 16, 8,
#ifdef GL_RGBA16UI
  64, 48, 32, 16,
#endif
#ifdef GL_RGBA32UI
  128, 96, 64, 32,
#endif
#ifdef GL_RGBA16I
  64, 48, 32, 16,
#endif
#ifdef GL_RGBA32I
  128, 96, 64, 32,
#endif
#ifdef GL_RGBA16F
  64, 48, 32, 16,
#endif
#ifdef GL_RGBA32F
  128, 96, 64, 32,
#endif
  0
};

/***********/


CogTexture CogTextureAlloc(const char* name, struct CogBundle* bundle) {
  CogTexture self = (CogTexture)malloc(sizeof(struct CogTexture));
  self->format = CogColorFormatRGBA;
  self->width = self->height = self->depth = 0;
  self->name = CogStringAlloc(name);
  self->internalFormat = CogColorInternalFormatRGBA8;
  glGenTextures(1, &self->id);
  self->anisotropicFiltering = False;
  self->anisotropyLevel = 1.0f;
  self->mipmaps = False;
  self->type = CogTextureTypeTexture2D;
  self->min = CogTextureFilteringNearest;
  self->mag = CogTextureFilteringNearest;
  
  if(bundle) {
    CogBundleAddTexture(bundle, self);
    CogLoggerInfo(CogPlatformGetLogger(), "[Resource] Texture <%s> created and added to CogBundle <%s>.\n", self->name->data, bundle->name->data);
  } else {
    CogLoggerInfo(CogPlatformGetLogger(), "[Resource] Texture <%s> created.\n", self->name->data);
  }
  
  return self;
}

CogTexture CogTextureAllocFromFile(const char* name, const char* file, struct CogBundle* bundle) {
  CogImage tmp = CogImageAllocFromFile(name, file, NULL);
  CogTexture self = CogImageCreateTexture(tmp, bundle);
  CogImageFree(tmp);
  return self;
}

void
CogTextureFree(
  CogTexture         self
)
{
  CogLoggerInfo(CogPlatformGetLogger(), "[Resource] Texture <%s> deleted.\n", self->name->data);
  CogStringFree(self->name);
  glDeleteTextures(1, &self->id);
  free(self);
}

void
CogTextureGenerateMipmaps(
  const CogTexture   self
)
{
  glBindTexture(self->type, self->id);
  glGenerateMipmap(self->type);
  glBindTexture(self->type, 0);
}

CogComponentType
CogTextureGetComponentType(
  const CogTexture   self
)
{
  CogComponentType type;
  if(self->internalFormat == CogColorInternalFormatR8 || self->internalFormat == CogColorInternalFormatRG8 ||self->internalFormat == CogColorInternalFormatRGB8 || self->internalFormat == CogColorInternalFormatRGBA8) {
    type = CogComponentTypeUByte;
  } 
  #ifdef GL_RGBA16UI
  else if(self->internalFormat == CogColorInternalFormatR16UI || self->internalFormat == CogColorInternalFormatRG16UI ||self->internalFormat == CogColorInternalFormatRGB16UI || self->internalFormat == CogColorInternalFormatRGBA16UI) {
    type = CogComponentTypeUShort;
  } 
  #endif
  #ifdef GL_RGBA32UI 
  else if(self->internalFormat == CogColorInternalFormatR32UI || self->internalFormat == CogColorInternalFormatRG32UI ||self->internalFormat == CogColorInternalFormatRGB32UI || self->internalFormat == CogColorInternalFormatRGBA32UI) {
    type = CogComponentTypeUInt;
  } 
  #endif
  #ifdef GL_RGBA16I
  else if(self->internalFormat == CogColorInternalFormatR16I || self->internalFormat == CogColorInternalFormatRG16I ||self->internalFormat == CogColorInternalFormatRGB16I || self->internalFormat == CogColorInternalFormatRGBA16I) {
    type = CogComponentTypeShort;
  } 
  #endif
  #ifdef GL_RGBA32I
  else if(self->internalFormat == CogColorInternalFormatR32I || self->internalFormat == CogColorInternalFormatRG32I ||self->internalFormat == CogColorInternalFormatRGB32I || self->internalFormat == CogColorInternalFormatRGBA32I) {
    type = CogComponentTypeInt;
  } 
  #endif
  #ifdef GL_RGBA16F
  else if(self->internalFormat == CogColorInternalFormatR16F || self->internalFormat == CogColorInternalFormatRG16F ||self->internalFormat == CogColorInternalFormatRGB16F || self->internalFormat == CogColorInternalFormatRGBA16F) {
    type = CogComponentTypeHalf;
  } 
  #endif
  #ifdef GL_RGBA32F
  else if(self->internalFormat == CogColorInternalFormatR32F || self->internalFormat == CogColorInternalFormatRG32F ||self->internalFormat == CogColorInternalFormatRGB32F || self->internalFormat == CogColorInternalFormatRGBA32F) {
    type = CogComponentTypeFloat;
  }
  #endif
  else {
    type = CogComponentTypeUndefined;
  }
  return type;
}

void
CogTextureInspect(
  const CogTexture   self
)
{
  int t, cf, cif, mf, Mf;
  const char* mip;
  const char* ani;
  for(t = 0; t < 4; ++t) { if(__cog_ttCogArray__[t] == self->type) break; }
  for(mf = 0; mf < 6; ++mf) { if(__cog_tfCogArray__[mf] == self->min) break; }
  for(Mf = 0; Mf < 6; ++Mf) { if(__cog_tfCogArray__[Mf] == self->mag) break; }
  for(cf = 0; cf < 4; ++cf) { if(__cog_cfCogArray__[cf] == self->format) break; }
  for(cif = 0; __cog_cfCogArray__[cif] != 0; ++cif) { if(__cog_cifCogArray__[cif] == self->internalFormat) break; }
  if(self->mipmaps) mip = "True"; else mip = "False";
  if(self->anisotropicFiltering) ani = "True"; else ani = "False";
  size_t sz = self->width * self->height * self->depth * __cog_szCogArray__[cif] / 8;
  
  printf(
"<Texture @0x%x\n"
"  name:                  %s\n"
"  type:                  %s\n"  
"  dimensions:            %d x %d x %d\n"
"  size:                  %lu bytes\n"
"  color format:          %s\n"
"  internal color format: %s\n"
"  minification filter:   %s\n"
"  maximization filter:   %s\n"
"  mipmaps:               %s\n"
"  anisotropy:            %s\n"
"  anisotropy level:      %f>\n",
         (unsigned int)self, self->name->data, __cog_ttnameCogArray__[t], self->width, self->height, self->depth, sz,
         __cog_cfnameCogArray__[cf], __cog_cifnameCogArray__[cif], __cog_tfnameCogArray__[mf], __cog_tfnameCogArray__[Mf], mip, ani, self->anisotropyLevel);
}