#version 330

#define LightTypeAmbient 1
#define LightTypePoint 2
#define LightTypeDirectional 3
#define LightTypeSpot 4

uniform sampler2D   texChannel[16];
uniform float       floatChannel[16];

uniform int lightType[16];
uniform vec4 lightColor[16];
uniform vec3 lightPosition[16];
uniform vec3 lightDirection[16];
uniform float lightConstantAttenuation[16];
uniform float lightLinearAttenuation[16];
uniform float lightQuadraticAttenuation[16];
uniform float lightInnerAngle[16];
uniform float lightOuterAngle[16];

in vec2 fTexCoord;
out vec4 fragcolor;

void main() {
  vec2 tc = vec2(fTexCoord.x, 1.0f - fTexCoord.y);
  
  vec4 position = texture(texChannel[0], tc);
  vec4 topography = texture(texChannel[1], tc);
  vec4 diffuse = texture(texChannel[2], tc);
  vec4 specular = texture(texChannel[3], tc);
  vec3 normal = normalize(vec3(topography.xyz * 2.0f - 1.0f));
  vec4 contrib = vec4(0.0f, 0.0f, 0.0f, 0.0f);
  
  // light contributions
  for(int i = 0; i < 16; ++i) {
    if(lightType[i] <= 0) { break; }
    
    /////////////////////
    //  Ambient Light  //
    /////////////////////
    else if(lightType[i] == LightTypeAmbient) {
      contrib += lightColor[i];	
    }
    
    ///////////////////
    //  Point Light  //
    ///////////////////
    else if(lightType[i] == LightTypePoint) {
      
      vec4 col = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        
      vec3 lightDir = position.xyz - lightPosition[i];
      float distance = length(lightDir);
      lightDir = -normalize(lightDir);
      float diffuseFactor = dot(lightDir, normal);
      float att = 1.0f / (lightConstantAttenuation[i] + lightLinearAttenuation[i] * distance + lightQuadraticAttenuation[i] * distance * distance);
      
      if(diffuseFactor > 0) {
        col += vec4(lightColor[i].rgb * diffuse.a * diffuseFactor, 1.0f);
        vec3 vertToEye = -normalize(position.xyz);
        vec3 lightReflect = normalize(reflect(-lightDir, normal));
        float specularFactor = dot(vertToEye, lightReflect);
        specularFactor = pow(specularFactor, specular.a * 256.0f);
        if(specularFactor > 0) {
          col += vec4(topography.a * specularFactor * specular.rgb, 1.0f);
        }
      }
      contrib += col * att;	
    }
    
    /////////////////////////
    //  Directional Light  //
    /////////////////////////
    else if(lightType[i] == LightTypeDirectional) {
      
      vec4 col = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        
      vec3 lightDir = -normalize(lightDirection[i]);
      float diffuseFactor = dot(lightDir, normal);
      
      if(diffuseFactor > 0) {
        col += vec4(lightColor[i].rgb * diffuse.a * diffuseFactor, 1.0f);
        vec3 vertToEye = -normalize(position.xyz);
        vec3 lightReflect = normalize(reflect(-lightDir, normal));
        float specularFactor = dot(vertToEye, lightReflect);
        specularFactor = pow(specularFactor, specular.a * 256.0f);
        if(specularFactor > 0) {
          col += vec4(topography.a * specularFactor * specular.rgb, 1.0f);
        }
      }
      contrib += col;	
    }
    
    ///////////////////
    //  Spot Light  //
    ///////////////////
    else if(lightType[i] == LightTypeSpot) {
      
      vec4 col = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        
      vec3 lightDir = position.xyz - lightPosition[i];
      float diffDotAngle = 1.0f - dot(-normalize(lightDir), lightDirection[i]);
      float alphai = lightInnerAngle[i];
      float alphao = lightOuterAngle[i];
      
      float darkenFactor = 0.0f;
      if(diffDotAngle < alphai) {
        darkenFactor = 1.0f;
      } else if(diffDotAngle >= alphai && diffDotAngle < alphao) {
        darkenFactor = 1.0f - (diffDotAngle - alphai) / (alphao - alphai);
      } else {
        darkenFactor = 0.0f;
      }
      
      if(darkenFactor > 0.0f) {
      
        float distance = length(lightDir);
        lightDir = -normalize(lightDir);
        float diffuseFactor = dot(lightDir, normal);
        
        if(diffuseFactor > 0) {
          col += vec4(lightColor[i].rgb * diffuse.a * diffuseFactor, 1.0f);
          vec3 vertToEye = -normalize(position.xyz);
          vec3 lightReflect = normalize(reflect(-lightDir, normal));
          float specularFactor = dot(vertToEye, lightReflect);
          specularFactor = pow(specularFactor, specular.a * 256.0f);
          if(specularFactor > 0) {
            col += vec4(topography.a * specularFactor * specular.rgb, 1.0f);
          }
        }
        contrib += col * darkenFactor;
      }

    }
  }
  //fragcolor = vec4((contrib * diffuse * (1.0f - position.a * position.a* position.a) + vec4(0.0f, 0.0f, 0.0f, 1.0f) * position.a * position.a * position.a).rgb, 1.0f);
  fragcolor = contrib * diffuse;
}
