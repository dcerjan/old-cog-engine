#version 330
////////////////
//  Material  //
////////////////
  /* Ambient */
  uniform sampler2D   ambientOcclusionMap;
  uniform float       ambientIntensity;
  uniform float       ambientOcclusionStrenght;
  uniform vec4        ambientColor;
  uniform int         hasOcclusionMap;
  /* Diffuse */
  uniform sampler2D   diffuseMap;
  uniform float       diffuseIntensity;
  uniform vec4        diffuseColor;
  uniform int         hasDiffuseMap;
  /* Specular */
  uniform sampler2D   specularMap;
  uniform float       specularIntensity;
  uniform float       specularHardness; 
  uniform vec4        specularColor;
  uniform int         hasSpecularMap;
  /* Bump */
  uniform sampler2D   normalMap;
  uniform sampler2D   heightMap;
  uniform float       heightScale;
  uniform int         hasNormalMap;
  uniform int         hasHeightMap;
  /* Reflection */
  uniform sampler2D   reflectionMap;
  uniform float       reflectionStrenght;
  uniform vec4        reflectionTint;
  uniform int         hasReflectionMap;
  /* Detail */
  uniform sampler2D   detailMap;
  uniform float       detailStrenght;
  uniform int         hasDetailMap;
  /* Glow */
  uniform sampler2D   glowMap;
  uniform float       glowStrenght;
  uniform vec4        glowColor;
  uniform int         hasGlowMap;

/////////////////////
//  From Pipeline  //
/////////////////////
  in vec4 fPosition;
  in vec4 fNormal;
  in vec4 fTangent;
  in vec2 fTexCoord;
  in float depth;

//////////////
//  Output  //
//////////////
  out vec4 fragcolor[4];

void main() {
  
  /* TBN */
  vec3 N = normalize(fNormal.xyz);
  vec3 T = normalize(fTangent.xyz);
  T = normalize(T - dot(N, T) * N);
  vec3 B = cross(N, T);
  mat3 TBN = mat3(T, B, N);
  
  fragcolor[0] = vec4(fPosition.xyz, depth);
  
  if(hasNormalMap == 1) {
    N = TBN * (2.0f * texture(normalMap, fTexCoord) - 1.0f).xyz;
    fragcolor[1] = vec4(0.5f + 0.5f * N, specularIntensity);
  } else {
    fragcolor[1] = vec4(0.5f + 0.5f * N.xyz, specularIntensity);
  }
  
  if(hasDiffuseMap == 1) {
    fragcolor[2] = vec4(texture(diffuseMap, fTexCoord).rgb * diffuseColor.rgb, diffuseIntensity);
  } else {
    fragcolor[2] = vec4(diffuseColor.rgb, diffuseIntensity);
  }
  
  if(hasSpecularMap == 1) {
    fragcolor[3] = vec4(texture(specularMap, fTexCoord).rgb * specularColor.rgb, specularHardness);
  } else {
    fragcolor[3] = vec4(specularColor.rgb, specularHardness);
  }
}