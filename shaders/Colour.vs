#version 330 core

layout(location = 0) in vec3 inVert;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

// UBO for transformation matrices (same as Phong shader)
layout(std140) uniform MatrixBlock {
    mat4 MVP;
    mat4 MV;
    mat4 M;
    mat3 normalMatrix;
    vec3 viewerPos;
    bool shouldNormalize;
};

void main() {
    gl_Position = MVP * vec4(inVert, 1.0);
}
