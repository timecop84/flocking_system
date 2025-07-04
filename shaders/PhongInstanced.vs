#version 450 core

// Input attributes (per vertex)
layout(location = 0) in vec3 inVert;    // Position
layout(location = 1) in vec2 inUV;      // Texture coordinates (unused)
layout(location = 2) in vec3 inNormal;  // Normal

// Instance attributes (per instance)
layout(location = 3) in mat4 instanceMatrix; // Model matrix for this instance (locations 3, 4, 5, 6)
layout(location = 7) in vec4 instanceColor;  // Color for this instance

// Output to fragment shader
out vec3 FragPos;      // Position in world space
out vec3 Normal;       // Normal in world space
out vec4 InstanceColor; // Color passed from instance data

// Uniform Buffer Object for matrices
layout(std140, binding = 0) uniform MatrixBlock {
    mat4 MVP;        // Base MVP (usually just View-Projection)
    mat4 MV;         // Base MV (usually just View)
    mat4 M;          // Base M (usually identity for instances)
    mat3 normalMatrix; // Normal matrix (computed per instance)
    vec3 viewerPos;  // Camera position in world space
    float shouldNormalize;
};

void main()
{
    // Calculate actual model-view matrix using instance matrix
    mat4 actualMV = MV * instanceMatrix;
    mat4 actualMVP = MVP * instanceMatrix;
    
    // Transform vertex to view space using instance matrix
    vec4 viewPos = actualMV * vec4(inVert, 1.0);
    FragPos = viewPos.xyz;
    
    // Transform normal to view space using instance matrix
    mat3 instanceNormalMatrix = mat3(transpose(inverse(actualMV)));
    Normal = normalize(instanceNormalMatrix * inNormal);
    
    // Pass through instance color
    InstanceColor = instanceColor;
    
    // Transform vertex to clip space
    gl_Position = actualMVP * vec4(inVert, 1.0);
}
