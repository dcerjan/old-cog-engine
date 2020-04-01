#version 330

uniform sampler2D   texChannel[1];

in vec2 fTexCoord;
out vec4 fragcolor;

void main() {
  float depth = texture(texChannel[0], vec2(fTexCoord.x, 1.0f - fTexCoord.y)).a;
  fragcolor = vec4(depth, depth, depth, 1.0f);
}
