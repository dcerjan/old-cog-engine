#ifndef _COG_IMAGE_FORMAT_H_
#define _COG_IMAGE_FORMAT_H_

#include "coggl.h"

typedef enum CogTextureFiltering {
  /* regular filtering */
  CogTextureFilteringNearest               = GL_NEAREST,
  CogTextureFilteringLinear                = GL_LINEAR,
  /* mipmap filtering */
  CogTextureFilteringNearestMipmapNearest  = GL_NEAREST_MIPMAP_NEAREST,
  CogTextureFilteringNearestMipmapLinear   = GL_NEAREST_MIPMAP_LINEAR,
  CogTextureFilteringLinearMipmapNearest   = GL_LINEAR_MIPMAP_NEAREST,
  CogTextureFilteringLinearMipmapLinear    = GL_LINEAR_MIPMAP_LINEAR,
} CogTextureFiltering;


typedef enum CogTextureType {
  CogTextureTypeTexture1D             = GL_TEXTURE_1D,
  CogTextureTypeTexture2D             = GL_TEXTURE_2D,
  CogTextureTypeTexture3D             = GL_TEXTURE_3D,
  CogTextureTypeTexture2DMultiSample  = GL_TEXTURE_2D_MULTISAMPLE,
  CogTextureTypeTextureCubeMap        = GL_TEXTURE_CUBE_MAP
} CogTextureType;


typedef enum CogColorFormat {
  CogColorFormatRGBA  = GL_RGBA,
  CogColorFormatRGB   = GL_RGB, 
  CogColorFormatRG    = GL_RG,
  CogColorFormatRED   = GL_RED
} CogColorFormat;


typedef enum CogComponentType {
  CogComponentTypeUByte = GL_UNSIGNED_BYTE,
  CogComponentTypeByte = GL_BYTE,
  CogComponentTypeUShort = GL_UNSIGNED_SHORT,
  CogComponentTypeShort = GL_SHORT,
  CogComponentTypeUInt = GL_UNSIGNED_INT,
  CogComponentTypeInt = GL_INT,
  CogComponentTypeHalf = GL_HALF_FLOAT,
  CogComponentTypeFloat = GL_FLOAT,
  CogComponentTypeUndefined = GL_FALSE
} CogComponentType;


typedef enum CogColorInternalFormat {
  /* Unsigned int formats */
  CogColorInternalFormatRGBA8       = GL_RGBA8,
  CogColorInternalFormatRGB8        = GL_RGB8,
  CogColorInternalFormatRG8         = GL_RG8,
  CogColorInternalFormatR8          = GL_R8,
#ifdef GL_RGBA16UI
  CogColorInternalFormatRGBA16UI    = GL_RGBA16UI,
  CogColorInternalFormatRGB16UI     = GL_RGB16UI,
  CogColorInternalFormatRG16UI      = GL_RG16UI,
  CogColorInternalFormatR16UI       = GL_R16UI,
#endif
#ifdef GL_RGBA32UI
  CogColorInternalFormatRGBA32UI    = GL_RGBA32UI,
  CogColorInternalFormatRGB32UI     = GL_RGB32UI,
  CogColorInternalFormatRG32UI      = GL_RG32UI,
  CogColorInternalFormatR32UI       = GL_R32UI,
#endif
  /* Signed int formats */
#ifdef GL_RGBA8I
  CogColorInternalFormatRGBA8I    = GL_RGBA8I,
  CogColorInternalFormatRGB8I     = GL_RGB8I,
  CogColorInternalFormatRG8I      = GL_RG8I,
  CogColorInternalFormatR8I       = GL_R8I,
#endif
#ifdef GL_RGBA16I
  CogColorInternalFormatRGBA16I   = GL_RGBA16I,
  CogColorInternalFormatRGB16I    = GL_RGB16I,
  CogColorInternalFormatRG16I     = GL_RG16I,
  CogColorInternalFormatR16I      = GL_R16I,
#endif
#ifdef GL_RGBA32I
  CogColorInternalFormatRGBA32I   = GL_RGBA32I,
  CogColorInternalFormatRGB32I    = GL_RGB32I,
  CogColorInternalFormatRG32I     = GL_RG32I,
  CogColorInternalFormatR32I      = GL_R32I,
#endif
  /* Floating point formats */
#ifdef GL_RGBA16F
  CogColorInternalFormatRGBA16F   = GL_RGBA16F,
  CogColorInternalFormatRGB16F    = GL_RGB16F,
  CogColorInternalFormatRG16F     = GL_RG16F,
  CogColorInternalFormatR16F      = GL_R16F,
#endif
#ifdef GL_RGBA32F
  CogColorInternalFormatRGBA32F   = GL_RGBA32F,
  CogColorInternalFormatRGB32F    = GL_RGB32F,
  CogColorInternalFormatRG32F     = GL_RG32F,
  CogColorInternalFormatR32F      = GL_R32F,
#endif
  CogColorInternalFormatUNDEFINED = GL_FALSE
} CogColorInternalFormat;

#endif