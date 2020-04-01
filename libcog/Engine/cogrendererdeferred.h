#ifndef _COG_RENDERER_DEFERRED_H_
#define _COG_RENDERER_DEFERRED_H_

#include "cogrenderer.h"
#include "cogtypes.h"


typedef struct CogRendererDeferredConfiguration {
  struct {
    float scale;
    Bool  useFP32PositionTarget;
  } deferred;
  
  struct {
    float scale;
  } bloom;
  
  struct {
    float scale;
  } screenSpace;
  
  struct {
    float scale;
  } composition;
  
  struct {
    int resolution;
  } shadow;
  
  struct {
    int width;
    int height;
  } resolution;
  
  struct {
    int maxLights;
  } lighting;
  
  struct {
    Bool debugShaderOutput;
  } debug;
} CogRendererDeferredConfiguration;


struct CogSurface;
struct CogFrameBuffer;
struct CogRenderTarget;
struct CogShader;
struct CogBundle;
struct CogCompositor;
struct CogTexture;
struct CogRendererDeferred {
  CogInherit(struct CogRenderer);
  
  CogRendererDeferredConfiguration   conf;
  struct {
    struct {
      Bool on;
      int numSamples;
      float radius;
      float constantAttenuation;
      float linearAttenuation;
      float quadraticAttenuation;
      float bias;
      struct {
        Bool on;
        int numSamples;
        float radius;
      } blur;
    } ssao;
    
    struct {
      Bool on;
      float lowerBound;
      float upperBound;
      struct {
        Bool on;
        int numSamples;
        float radius;
      } blur;
    } edgeDetect;
    
    struct {
      Bool on;
      struct {
        Bool on;
        int numSamples;
        float radius;
        float lowerBound;
        float upperBound;
      } edgeDetect;
      struct {
        int numSamples;
        float radius;
      } blur;
      int numSamples;
    } ssaa;
    
    struct {
      Bool on;
      int numMarches;
      int numMaxRayBounces;
      struct {
        Bool on;
        int numSamples;
        float radius;
      } blur;
    } ssr;
    
    struct {
      Bool on;
    } bloom;
    
  } videoOptions;
  
  struct CogSurface*       surface;
  
  struct {
    struct CogFrameBuffer* bloomFbo;
    struct CogFrameBuffer* shadowMapFbo;
    struct CogFrameBuffer* screenSpaceFbo;
    struct CogFrameBuffer* deferredFbo;
    struct CogFrameBuffer* compositionFbo;
  } fbo;
  
  struct CogTexture*       randommap;
  
  struct CogBundle*        store;
  
  struct CogCompositor*    compositor;
  
  struct {
    struct CogRenderTarget*  position;
    struct CogRenderTarget*  normal;
    struct CogRenderTarget*  diffuse;
    struct CogRenderTarget*  specular;
  } rt;
  
  
  struct {
    /* debug surfaces */
    struct CogSurface*       positionSurface;
    struct CogSurface*       depthSurface;
    struct CogSurface*       topologySurface;
    struct CogSurface*       diffuseSurface;
    struct CogSurface*       specularSurface;
    
    /* debug shaders */
    struct CogShader*        position;
    struct CogShader*        depth;
    struct CogShader*        topology;
    struct CogShader*        diffuse;
    struct CogShader*        specular;
  } debug;
  
  struct {
    struct {
      struct {
        struct {
          struct CogShader* rgba;
          struct CogShader* rgb;
          struct CogShader* rg;
          struct CogShader* r;
        } gaussian;
        struct {
          struct CogShader* rgba;
          struct CogShader* rgb;
          struct CogShader* rg;
          struct CogShader* r;
        } box;
      } blur;
      struct {
        struct {
          struct CogShader* depth;
          struct CogShader* depthNormal;
        } freiChen;
        struct {
          struct CogShader* depth;
          struct CogShader* depthNormal;
        } sobel;
      } edgeDetect;
      struct CogShader* blit;
      struct CogShader* composer;
    } util;
    
    struct {
      struct CogShader* ambientOcclusion;
      struct CogShader* reflection;
      struct CogShader* bloom;
      struct CogShader* godRays;
    } effect;
    
    struct {
      struct CogShader* bake;
      struct CogShader* compose;
      struct CogShader* composeDeferred;
    } deferred;
    
  } shaders;
};
typedef struct CogRendererDeferred* CogRendererDeferred;

CogRenderer CogRendererDeferredAlloc       (CogRendererDeferredConfiguration conf);
void        CogRendererDeferredReconfigure (CogRendererDeferredConfiguration conf);
void        CogRendererDeferredFree        (CogRenderer self);

#endif