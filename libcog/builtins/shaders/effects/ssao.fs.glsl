#version 330

uniform sampler2D   texChannel[3];
uniform float       floatChannel[6];
uniform int         intChannel[1];

#define PositionMap           0
#define NormalMap             1
#define PerturbMap            2

#define OccluderBias          0
#define SamplingRadius        1
#define ConstantAttenuation   2
#define LinearAttenuation     3
#define QuadraticAttenuation  4

#define NumSamples            0

in vec2 fTexCoord;
out vec4 fragcolor;

const float sin45 = 0.707107f;
const vec2 kernel[4] = vec2[](
  vec2(0.0f, 1.0f),
  vec2(1.0f, 0.0f),
  vec2(0.0f, -1.0f),
  vec2(-1.0f, 0.0f)
);

float ssaoContribution(vec4 position, vec3 normal, vec2 uv) {
  vec4 dstPosition = texture(texChannel[PositionMap], uv);
  vec3 vecFromTo = dstPosition.xyz - position.xyz;
  float intensity = max(dot(normalize(vecFromTo), normal) - floatChannel[OccluderBias], 0.0f);
  float len = length(vecFromTo);
  float attenuation = 1.0f / (floatChannel[ConstantAttenuation] + len * floatChannel[LinearAttenuation] + len * len * floatChannel[QuadraticAttenuation]);
  return intensity * attenuation;
}

void main() {
  vec4 color = vec4(0.0f);
  vec2 tc = vec2(fTexCoord.x, 1.0f - fTexCoord.y);
  vec4 pos = texture(texChannel[PositionMap], tc);
  vec3 normal = texture(texChannel[NormalMap], tc).xyz * 2.0f - 1.0f;
  vec2 rnd = ((texture(texChannel[PerturbMap], tc).xyz * 2.0f - 1.0f).xz);
  float depth = length(pos.xyz);
  float kernelRadius = floatChannel[SamplingRadius] * (1.0f / (depth));
  ivec2 texSize = textureSize(texChannel[PositionMap], 0);
  vec2 texelSize = vec2(1.0f / float(texSize.x), 1.0f / float(texSize.y));
  
  int nSamples;
  if(intChannel[NumSamples] <= 16 || intChannel[NumSamples] > 256) {
    nSamples = 64;
  } else {
    nSamples = intChannel[NumSamples];
  }
  nSamples /= 16; 
  
  for(int i = 0; i < 4; ++i) {
    vec2 k1 = reflect(kernel[i], rnd);
    vec2 k2 = vec2(k1.x * sin45 - k1.y * sin45, k1.x * sin45 + k1.y * sin45);
    
    k1 *= texelSize;
    k2 *= texelSize;
    
    for(int j = 0; j < nSamples; ++j) {
      color += ssaoContribution(pos, normal, tc + k1 * kernelRadius * (float(2 * j + 1) / nSamples));
      color += ssaoContribution(pos, normal, tc + k2 * kernelRadius * (float(2 * j + 2) / nSamples));
    }
  }
  
  color /= nSamples * 16.0f;
  color = clamp(color, 0.0f, 1.0f);
  
  fragcolor = vec4(color.rgb, 1.0f);
}