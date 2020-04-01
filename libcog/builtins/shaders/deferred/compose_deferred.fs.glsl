#version 330

uniform sampler2D   texChannel[16];
uniform float       floatChannel[16];
uniform int         intChannel[16];

in vec2 fTexCoord;
out vec4 fragcolor;

#define COMPOSED_MAP 0
#define SSAO_MAP 1
#define EDGEDETECT_MAP 2
#define SSAA_MAP 3
#define SSR_MAP 4
#define BLOOM_MAP 6

void main() {
  int i;
  vec4 color;
  vec2 uv = vec2(fTexCoord.x, 1.0f - fTexCoord.y);
  
  if(intChannel[SSAA_MAP] + intChannel[EDGEDETECT_MAP] == 2) {
    float fac = texture(texChannel[EDGEDETECT_MAP], uv).r;
    color = (1.0f - fac) * texture(texChannel[COMPOSED_MAP], uv) + (fac) * texture(texChannel[SSAA_MAP], uv);
  } else {
    color = texture(texChannel[COMPOSED_MAP], uv);
  }
  
  /*
  if(intChannel[SSR_MAP] == 1) {
    color = 0.5f * (color + texture(texChannel[SSR_MAP]));
  }
  */
  
  if(intChannel[SSAO_MAP] == 1) {
    float ssao = texture(texChannel[SSAO_MAP], uv).r;
    color -= vec4(ssao, ssao, ssao, 0.0f);
  }
  
  /*
  if(intChannel[EDGEDETECT_MAP] == 1) {
    float edgeDetect = texture(texChannel[EDGE_DETECT_MAP], uv).r;
    color -= vec4(edgeDetect, edgeDetect, edgeDetect, 0.0f);
  }
  */
  
  if(intChannel[BLOOM_MAP] == 1) {
    color += texture(texChannel[BLOOM_MAP], uv);
  } 
  
  fragcolor = color;
}
