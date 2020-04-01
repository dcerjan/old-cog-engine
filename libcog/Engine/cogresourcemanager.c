#include "cogresourcemanager.h"
#include "cogplatform.h"
#include "coggc.h"

#include "cogjson.h"

static CogResourceManager   __cog_resource_manager__ = NULL;
static CogCleanupPool       __cog_resource_manager_pool__ = NULL;

///////////////
//  Helpers  //
///////////////
CogArray(char) getBytesFromFile(const char* name) {
  FILE* fp = fopen(name, "r");
  if(!fp) {
    CogLoggerError(CogPlatformGetLogger(), "Unable to open file <%s>.", name);
    exit(2);
  }
  
  CogArray(char) buffer = CogArrayAlloc(char);
  
  char c;
  while(c = fgetc(fp), c != EOF) {
    CogArrayPush(buffer, c);
  }
  CogArrayPush(buffer, '\0');
  
  return buffer;
}

////////////////////////
//  Resource Manager  //
////////////////////////
void
CogResourceManagerInit(
  void
)
{
  if(__cog_resource_manager__) {
    CogLoggerLog(CogPlatformGetLogger(), "[Error]", "Cog Resource Manager is already initialized!\n");
    exit(-1);
  }
  __cog_resource_manager__ = (CogResourceManager)malloc(sizeof(struct CogResourceManager));
  __cog_resource_manager__->bundles = CogHashAlloc(CogBundle);
  __cog_resource_manager_pool__ = CogCleanupPoolAlloc("Resource Manager Pool");
}

void
CogResourceManagerCleanup(
  void
)
{
  CogCleanupPoolFree(__cog_resource_manager_pool__);
  free(__cog_resource_manager__);
}

CogResourceManager
CogResourceManagerGet(
  void
)
{
  if(!__cog_resource_manager__) {
    CogLoggerLog(CogPlatformGetLogger(), "[Error]", "Cog Resource Manager is not be initialized!\n");
    exit(-1);
  }
  return __cog_resource_manager__;
}


////////////////
//  Cog CogMesh  //
////////////////
void CogResourceManagerSerializeCogMesh(const CogMesh self, const char* destination);


////////////////////
//  Cog Material  //
////////////////////
void _freeMaterial(CogMaterial self);
CogMaterial _allocMaterial(void);
CogMaterial _loadMaterial(CogJsonObject json);
void CogResourceManagerSerializeMaterial(const CogMaterial self, const char* destination);

/////////////////
//  Cog Model  //
/////////////////
void _freeModel(CogModel self);
CogModel _allocModel(void);
CogModel _loadModel(CogJsonObject json);
void CogResourceManagerSerializeModel(const CogModel self, const char* destination);

//////////////////
//  Cog Shader  //
//////////////////
GLuint _compileFromFile(const char* file, GLenum type) {
  FILE* fp = fopen(file, "r");
  if(!fp) {
    CogLoggerError(CogPlatformGetLogger(), "shader source file '%s' not found.\n", file);
    exit(-1);
  }
  
  GLuint id = glCreateShader(type);
  
  char* buffer = malloc(sizeof(char) * 256);
  char c = '\0';
  int taken = 0;
  int size = 256;
  while(c = fgetc(fp), c != EOF) {
    if(taken == size -1) {
      size *= 2;
      buffer = realloc(buffer, sizeof(char) * size);
    }
    buffer[taken++] = c;
  }
  buffer[taken] = '\0';
  glShaderSource(id, 1, (const GLchar**)(&buffer), &taken);
  glCompileShader(id);
  
  GLint result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE) {
      char *gllog;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &taken);
      gllog = malloc(taken);
      glGetShaderInfoLog(id, taken, &result, gllog);
      CogLoggerError(CogPlatformGetLogger(), "Unable to compile shader source '%s', reason:\n%s\n", file, gllog);
      free(gllog);
      glDeleteShader(id);
      exit(-1);
  }
  free(buffer);
  fclose(fp);
  return id;
}
CogShader _loadShader(CogJsonObject json, CogBundle bundle) {
  // if we have a path reference specified
  CogString name, vert, geom, frag;
  vert = geom = frag = name = NULL;
  if(json->type == CogJsonObjectTypeString) {
    
  } else if(json->type == CogJsonObjectTypeObject){
    name = CogJsonObjectGetString(json, "name");
    vert = CogJsonObjectGetString(json, "vertex");
    geom = CogJsonObjectGetString(json, "geometry");
    frag = CogJsonObjectGetString(json, "fragment");
  }
  
  if(name == NULL) {
    CogLoggerError(CogPlatformGetLogger(), "Unable parse shader name.\n");
    exit(3);
  }
  
  if(!vert || !frag) {
    CogLoggerError(CogPlatformGetLogger(), "CogVertex and fragment shader stage can not be ommited for shader <%s>.\n", name->data);
    exit(3);
  }
  
  CogArray(char) vsource, gsource, fsource;
  vsource = gsource = fsource = NULL;
  
  CogShader shader;
  vsource = getBytesFromFile(vert->data);
  fsource = getBytesFromFile(frag->data);
  if(geom) { 
    gsource = getBytesFromFile(geom->data); 
    shader = CogShaderAlloc(name->data, (const char**)&vsource->data, (const char**)&gsource->data, (const char**)&fsource->data, bundle);
    CogArrayFree(vsource);
    CogArrayFree(gsource);
    CogArrayFree(fsource);
  } else {
    shader = CogShaderAlloc(name->data, (const char**)&vsource->data, NULL, (const char**)&fsource->data, bundle);
    CogArrayFree(vsource);
    CogArrayFree(fsource);
  }
  
  return shader;
}

/////////////////
//  Cog Light  //
/////////////////
void _freeLight(CogLight self);
CogLight _allocLight();
CogLight _loadLight(CogJsonObject json);
void CogResourceManagerSerializeLight(const CogLight self, const char* destination);

////////////////////
//  Cog Skeleton  //
////////////////////
void _freeSkeleton(CogSkeleton self);
CogSkeleton _allocSkeleton();
CogSkeleton _loadSkeleton(CogJsonObject json);
void CogResourceManagerSerializeSkeleton(const CogSkeleton, const char* destination);

///////////////////
//  Cog Texture  //
///////////////////
void _freeTexture(CogTexture self);
CogTexture _allocTexture(void);
CogTexture _loadTexture(CogJsonObject json);
void ResourceManagerSerializeTexture(const CogTexture, const char* destination);

/////////////////
//  Cog CogImage  //
/////////////////
void _freeCogImage(CogImage self);
CogImage _allocCogImage(void);
CogImage _loadCogImage(CogJsonObject json);
void ResourceManagerSerializeCogImage(const CogImage, const char* destination);

////////////////////////
//  Cog Audio Source  //
////////////////////////
void _freeCogAudioSource(CogAudioSource self);
CogAudioSource _allocCogAudioSource(void);
CogAudioSource _loadCogAudioSource(CogJsonObject json);

////////////////
//  Cog Font  //
////////////////
void _freeFont(CogFont self);
CogFont _allocFont(void);
CogFont _loadFont(CogJsonObject json);


//////////////////
//  Cog CogBundle  //
//////////////////
CogBundle CogResourceManagerAddBundle(CogBundle bundle) {
  CogHashSet(__cog_resource_manager__->bundles, bundle->name->data, bundle);  
  CogLoggerInfo(CogPlatformGetLogger(), "Bundle <%s> added.\n", bundle->name->data);
  CogCleanupPoolRegisterForCleanup(__cog_resource_manager_pool__, bundle, (auto_destruct)CogBundleFree);
  return bundle;
}

CogBundle CogResourceManagerGetBundle(const char* name) {
  assert(CogHashContains(__cog_resource_manager__->bundles, name));
  return CogHashGet(__cog_resource_manager__->bundles, name);
}

void CogResourceManagerRemoveBundle(const char* name) {
  CogLoggerInfo(CogPlatformGetLogger(), "Bundle <%s> cleanup started:\n", name);
  CogBundle bundle = CogHashGet(__cog_resource_manager__->bundles, name);
  CogHashRemove(__cog_resource_manager__->bundles, name);
  CogBundleFree(bundle);
  CogLoggerInfo(CogPlatformGetLogger(), "Bundle <%s> deleted.\n", name);
}

///////////////////////////
//  Cog Resource CogBundle  //
///////////////////////////
void CogResourceManagerLoadBundle(const char* path) {
  CogJsonObject bnd = CogJsonParserParseFromFile(NULL, path);
  
  if(!bnd) {
    CogLoggerError(CogPlatformGetLogger(), "Error parsing CogBundle '%s'\n\t in %s, function %s, line %d", path, __FILE__, __FUNCTION__, __LINE__);
  }
  
  CogString name = CogJsonObjectGetString(bnd, "name");
  CogJsonObject resources = CogJsonObjectGetObject(bnd, "resources");
  
  CogBundle bundle = CogResourceManagerAddBundle(CogBundleAlloc(name->data));
  
  // always parse 'raw-ish' formats first so dependant formats can use those resoruces
  // parse shaders
  CogArray(CogJsonObject) shaders = CogJsonObjectGetCogArray(resources, "shaders");
  CogArrayForEach(shaders, s, {
    CogShader shader = _loadShader(s, bundle);
    CogLoggerInfo(CogPlatformGetLogger(), "[Resource] Shader <%s> succesfully loaded to CogBundle <%s>.\n", shader->name->data, bundle->name->data);
  });
  
  // parse images
  
  // parse textures
  
  // parse materials (deps: texture/image, shader)
  
  // parse mesh
  
  // parse skeleton
  
  // parse model (deps: mesh, skeleton, material)
  
  // parse font
  
  // parse light
  
  // parse CogCamera
  
  // parse audio source
  
  // parse scene (deps: CogCamera, light, model, audiosource)
 
  CogJsonObjectFree(bnd);
}
