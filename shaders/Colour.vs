#version 330 core

layout(location = 0) in vec3 inVert;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(inVert, 1.0);
}
