#version 330

uniform sampler2D   texChannel[1];
uniform int         intChannel[2];
uniform float       floatChannel[1];

in vec2 fTexCoord;
out vec4 fragcolor;

float gaussian(float x, float deviation) {
  return (1.0f / sqrt(2.0f * 3.141592f * deviation)) * exp(-((x * x) / (2.0 * deviation)));
}

void main() {
  float halfBlur = float(intChannel[0]) * 0.5f;
  vec4 color = vec4(0.0f);
  
  float deviation = halfBlur * 0.35f;
  deviation *= deviation;
  
  vec2 tc = vec2(fTexCoord.x, 1.0f - fTexCoord.y);
  ivec2 texSize = textureSize(texChannel[0], 0);
  
  if(intChannel[1] == 0) {
    // horizonal blur
    for(int i = 0; i < 32; ++i) {
      if(i > intChannel[0]) { break; }
      float offset = (float(i) - halfBlur) / float(intChannel[0]);
      color += texture(texChannel[0], tc + vec2(offset * floatChannel[0] / float(texSize.x), 0.0f)).rgba * gaussian(offset, deviation);
    }
  } else {
    // vertical blur
    for(int i = 0; i < 32; ++i) {
      if(i > intChannel[0]) { break; }
      float offset = (float(i) - halfBlur) / float(intChannel[0]);
      color += texture(texChannel[0], tc + vec2(0.0f, offset * floatChannel[0] / float(texSize.y))).rgba * gaussian(offset, deviation);
    }
  }
  fragcolor = color;
}