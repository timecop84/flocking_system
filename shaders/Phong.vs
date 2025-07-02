#version 330 core

// Input attributes
layout(location = 0) in vec3 inVert;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

// UBO for transformation matrices
layout(std140) uniform MatrixBlock {
    mat4 MVP;
    mat4 MV;
    mat4 M;
    mat3 normalMatrix;
    vec3 viewerPos;
    float shouldNormalize;
};

// UBO for material properties
layout(std140) uniform MaterialBlock {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float padding[3];   // Padding for alignment
} material;

// UBO for lighting properties
layout(std140) uniform LightBlock {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    float spotCosCutoff;
} light;

// Output to fragment shader
out vec3 FragPos;
out vec3 Normal;

void main() {
    // Calculate world position
    FragPos = vec3(M * vec4(inVert, 1.0));
    
    // Transform normal to world space using the normal matrix
    // This is the correct way to handle normals for transformed geometry
    Normal = normalMatrix * inNormal;
    
    // Calculate final position
    gl_Position = MVP * vec4(inVert, 1.0);
}
