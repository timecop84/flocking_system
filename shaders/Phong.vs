#version 450 core

// Input attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

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
    // Transform position to world space using model matrix
    FragPos = vec3(M * vec4(aPosition, 1.0));
    
    // Transform normal to world space using the normal matrix
    // The normal matrix correctly handles non-uniform scaling
    Normal = normalize(normalMatrix * aNormal);
    
    // Final position in clip space using the combined MVP matrix
    gl_Position = MVP * vec4(aPosition, 1.0);
}
