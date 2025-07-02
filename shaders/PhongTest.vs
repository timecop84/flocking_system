#version 330 core

layout(location = 0) in vec3 inVert;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

// Matrices as simple uniforms for testing
uniform mat4 u_MVP;
uniform mat4 u_ModelMatrix;
uniform mat3 u_NormalMatrix;
uniform vec3 u_LightPos;
uniform vec3 u_ViewerPos;

out vec3 v_WorldPos;
out vec3 v_Normal;
out vec3 v_LightDir;
out vec3 v_ViewDir;

void main() {
    // Transform vertex to world space
    vec4 worldPosition = u_ModelMatrix * vec4(inVert, 1.0);
    v_WorldPos = worldPosition.xyz;
    
    // Transform normal to world space
    v_Normal = normalize(u_NormalMatrix * inNormal);
    
    // Calculate lighting vectors in world space
    v_LightDir = normalize(u_LightPos - v_WorldPos);
    v_ViewDir = normalize(u_ViewerPos - v_WorldPos);
    
    // Final position
    gl_Position = u_MVP * vec4(inVert, 1.0);
}
