#version 330

uniform sampler2D   texChannel[16];
uniform float       floatChannel[16];
uniform int         intChannel[16];

in vec2 fTexCoord;
out vec4 fragcolor;

void main() {
  int i;
  vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  vec2 uv = vec2(fTexCoord.x, 1.0f - fTexCoord.y);
  
  for(i = 0; i < 16; i++) {
    if(i >= intChannel[0]) {
      break;
    }
    color += floatChannel[i] * texture(texChannel[i], uv);
  }
  fragcolor = vec4(color.rgb, 1.0f);
}
