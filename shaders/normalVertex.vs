#version 150
precision highp float;
/// @brief the vertex passed in
in vec3 inVert;
/// @brief the normal passed in
in vec3 inNormal;
/// @brief the in uv
in vec2 inUV;
uniform mat4 MVP;

uniform float normalSize;
uniform vec4 vertNormalColour;
uniform vec4 faceNormalColour;

out vec4 normal;

uniform bool drawFaceNormals;
uniform bool drawVertexNormals;

void main(void)
{
  gl_Position = MVP*vec4(inVert,1);
  normal=MVP*vec4(inNormal,0);
}
