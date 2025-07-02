#version 330 core

layout(location = 0) in vec3 inVert;

// Simple UBO test
layout(std140) uniform TestBlock {
    mat4 MVP;
} testUBO;

void main() {
    gl_Position = testUBO.MVP * vec4(inVert, 1.0);
}
