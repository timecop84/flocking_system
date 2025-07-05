#version 450 core

// Input attributes
layout(location = 0) in vec3 inVert;    // Position
layout(location = 1) in vec2 inUV;     // Texture coordinates (unused)
layout(location = 2) in vec3 inNormal;  // Normal

// Output to fragment shader
out vec3 FragPos;    // Position in world space
out vec3 Normal;     // Normal in world space

// Uniform Buffer Object for matrices
layout(std140, binding = 0) uniform MatrixBlock {
    mat4 MVP;        // Model-View-Projection matrix
    mat4 MV;         // Model-View matrix
    mat4 M;          // Model matrix (object → world)
    mat3 normalMatrix; // Normal matrix
    vec3 viewerPos;  // Camera position in world space
    float shouldNormalize;
};

void main()
{
    // Transform vertex to view space (like fixed function pipeline)
    vec4 viewPos = MV * vec4(inVert, 1.0);
    FragPos = viewPos.xyz;
    
    // Transform normal to view space (like fixed function)
    Normal = normalize(mat3(MV) * inNormal);
    
    // Output position in clip space
    gl_Position = MVP * vec4(inVert, 1.0);
}
