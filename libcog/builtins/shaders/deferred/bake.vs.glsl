#version 330

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float linearDepthConstant;

out vec4 fPosition;
out vec4 fNormal;
out vec4 fTangent;
out vec2 fTexCoord;
out float depth;

void main() {
  mat4 mv = view * model;

  fPosition = mv * vec4(vPosition, 1.0f);
  fNormal = mv * vec4(normalize(vNormal), 0.0f);
  fTangent = mv * vec4(normalize(vTangent), 0.0f);
  fTexCoord = vTexCoord;
  depth = -fPosition.z * linearDepthConstant;
  
  gl_Position = projection * fPosition;
}