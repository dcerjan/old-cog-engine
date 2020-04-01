#version 330

uniform sampler2D   texChannel[16];
uniform float       floatChannel[16];

in vec2 fTexCoord;
out vec4 fragcolor;

// constants

float sqrt2 = 1.41421f;

// Convolution matrices
mat3x3 G[9] = mat3x3[](
  mat3x3(1.0f, sqrt2, 1.0f,
         0.0f, 0.0f, 0.0f,
         -1.0f, -sqrt2, -1.0f),
                       
  mat3x3(1.0f, 0.0f, -1.0f,
         sqrt2, 0.0f, -sqrt2,
         1.0f, 0.0f, -1.0f),
                       
  mat3x3(0.0f, -1.0f, sqrt2,
         1.0f, 0.0f, -1.0f,
         -sqrt2, 1.0f, 0.0f),
                       
  mat3x3(sqrt2, -1.0f, 0.0f,
         -1.0f, 0.0f, 1.0f,
         0.0f, 1.0f, -sqrt2),
                       
  mat3x3(0.0f, 1.0f, 0.0f,
         -1.0f, 0.0f, -1.0f,
         0.0f, 1.0f, 0.0f),
                       
  mat3x3(-1.0f, 0.0f, 1.0f,
         0.0f, 0.0f, 0.0f,
         1.0f, 0.0f, -1.0f),
                       
  mat3x3(1.0f, -2.0f, 1.0f,
         -2.0f, 4.0f, -2.0f,
         1.0f, -2.0f, 1.0f),
                       
  mat3x3(-2.0f, 1.0f, -2.0f,
         1.0f, 4.0f, 1.0f,
         -2.0f, 1.0f, -2.0f),
                       
  mat3x3(1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f)
);

// Convolutiion matrix weights
float g[9] = float[](
  0.353553f,
  0.353553f,
  0.353553f,
  0.353553f,
  0.5f,
  0.5f,
  0.166667f,
  0.166667f,
  0.333333f
);

void main() {
  float s = 0.0f;
  float m = 0.0f;
  
  vec2 tc = vec2(fTexCoord.x, 1.0f - fTexCoord.y);
  
  ivec2 texSize = textureSize(texChannel[0], 0);
  float d =  1.0f;
  
  float sample[9];
  sample[0] = length(texture(texChannel[0], tc + vec2( + (1.0f / texSize.x) * d, + (1.0f / texSize.y) * d)).rgba);
  sample[1] = length(texture(texChannel[0], tc + vec2(                     0.0f, + (1.0f / texSize.y) * d)).rgba);
  sample[2] = length(texture(texChannel[0], tc + vec2( - (1.0f / texSize.x) * d, + (1.0f / texSize.y) * d)).rgba);
  sample[3] = length(texture(texChannel[0], tc + vec2( + (1.0f / texSize.x) * d,                     0.0f)).rgba);
  sample[4] = length(texture(texChannel[0], tc + vec2(                     0.0f,                     0.0f)).rgba);
  sample[5] = length(texture(texChannel[0], tc + vec2( - (1.0f / texSize.x) * d,                     0.0f)).rgba);
  sample[6] = length(texture(texChannel[0], tc + vec2( + (1.0f / texSize.x) * d, - (1.0f / texSize.y) * d)).rgba);
  sample[7] = length(texture(texChannel[0], tc + vec2(                     0.0f, - (1.0f / texSize.y) * d)).rgba);
  sample[8] = length(texture(texChannel[0], tc + vec2( - (1.0f / texSize.x) * d, - (1.0f / texSize.y) * d)).rgba);
  
  int i;
  for(i = 0; i < 4; i++) {
    m += g[i] * (
      pow(G[i][0][0] * sample[0], 2.0f) +
      pow(G[i][1][0] * sample[1], 2.0f) +
      pow(G[i][2][0] * sample[2], 2.0f) +
      pow(G[i][0][1] * sample[3], 2.0f) +
      pow(G[i][1][1] * sample[4], 2.0f) +
      pow(G[i][2][1] * sample[5], 2.0f) +
      pow(G[i][0][2] * sample[6], 2.0f) +
      pow(G[i][1][2] * sample[7], 2.0f) +
      pow(G[i][2][2] * sample[8], 2.0f)
    );
  }
  
  for(i = 0; i < 9; i++) {
    s += g[i] * (
      pow(G[i][0][0] * sample[0], 2.0f) +
      pow(G[i][1][0] * sample[1], 2.0f) +
      pow(G[i][2][0] * sample[2], 2.0f) +
      pow(G[i][0][1] * sample[3], 2.0f) +
      pow(G[i][1][1] * sample[4], 2.0f) +
      pow(G[i][2][1] * sample[5], 2.0f) +
      pow(G[i][0][2] * sample[6], 2.0f) +
      pow(G[i][1][2] * sample[7], 2.0f) +
      pow(G[i][2][2] * sample[8], 2.0f)
    );
  }
  float e = acos(sqrt(m / s));
  if(e <= floatChannel[0]) {
    e = 0.0f;
  }
  if(e > floatChannel[1]) {
    e = 1.0f;
  }
  fragcolor = vec4(e, e, e, 1.0f);
}