#version 450 core

// Inputs from vertex shader (in world space)
in vec3 FragPos;     // Fragment position in world space
in vec3 Normal;      // Surface normal in world space

// Output color
out vec4 FragColor;

// Uniform Buffer for matrices (binding point 0)
layout(std140, binding = 0) uniform MatrixBlock {
    mat4 MVP;        // Model-View-Projection matrix
    mat4 MV;         // Model-View matrix
    mat4 M;          // Model matrix
    mat3 normalMatrix; // Normal matrix
    vec3 viewerPos;  // Camera position in world space
    float shouldNormalize;
};

// Uniform Buffer for lighting (binding point 3)
layout(std140, binding = 3) uniform LightingBlock {
    vec3 lightPos;   // Light position in world space
    float pad1;      // Padding
    vec3 viewPos;    // Camera position (redundant with viewerPos above)
    float pad2;      // Padding
    vec3 lightColor; // Light color
    float shininess; // Material shininess
};

void main()
{
    // Use viewerPos from the MatrixBlock (more reliable)
    vec3 viewerPosition = viewerPos;
    
    // Ambient component
    vec3 ambient = 0.2 * lightColor;  // Increased ambient for better visibility

    // Diffuse component
    vec3 norm = normalize(Normal);     // Normalize again to be safe
    vec3 lightDir = normalize(lightPos - FragPos);  // Direction to light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular component (Blinn-Phong for better highlights)
    vec3 viewDir = normalize(viewerPosition - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    // Final combined color
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
