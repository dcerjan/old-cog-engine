#version 330

uniform sampler2D   texChannel[1];

in vec2 fTexCoord;
out vec4 fragcolor;

void main() {
  vec3 color = texture(texChannel[0], vec2(fTexCoord.x, 1.0f - fTexCoord.y)).rgb;
  fragcolor = vec4(color, 1.0f);
}
