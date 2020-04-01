#version 330

precision highp float;
layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec2 vTexCoord;

out vec2 fTexCoord;

void main() {
  fTexCoord = vTexCoord;
  gl_Position = vec4(vPosition, 0.0f, 1.0f);
}
