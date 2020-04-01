#version 330

uniform sampler2D   texChannel[1];

in vec2 fTexCoord;
out vec4 fragcolor;

void main() {
  vec4 normal = texture(texChannel[0], vec2(fTexCoord.x, 1.0f - fTexCoord.y)) * 2.0f - 1.0f;
  fragcolor = vec4(normal.xyz, 1.0f);
}
