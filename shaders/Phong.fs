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

// Material properties UBO (binding point 1)
layout(std140, binding = 1) uniform MaterialBlock {
    vec4 ambient;    // Ambient material color
    vec4 diffuse;    // Diffuse material color
    vec4 specular;   // Specular material color
    float shininess; // Material shininess
    float padding[3]; // Padding for alignment
};

// Lighting properties UBO (binding point 3)
layout(std140, binding = 3) uniform LightingBlock {
    vec3 lightPos;   // Light position in world space
    float pad1;      // Padding
    vec3 viewPos;    // Camera position (redundant with viewerPos above)
    float pad2;      // Padding
    vec3 lightColor; // Light color
    float lightShininess; // Light-specific shininess (unused, use material.shininess)
};

void main()
{
    // Use viewerPos from the MatrixBlock (more reliable)
    vec3 viewerPosition = viewerPos;
    
    // Make sure we have a normalized normal vector
    vec3 norm = normalize(Normal);
    
    // Calculate view and light directions
    vec3 viewDir = normalize(viewerPosition - FragPos);
    vec3 lightDir = normalize(lightPos - FragPos);
    
    // Ambient component - use material ambient color
    vec3 ambientColor = ambient.rgb * lightColor * 0.2;
    
    // Diffuse component - use material diffuse color
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diffuse.rgb * lightColor * diff;
    
    // Specular component (Blinn-Phong for smoother highlights)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specularColor = specular.rgb * lightColor * spec;
    
    // Final combined color with proper material properties
    vec3 result = ambientColor + diffuseColor + specularColor;
    
    // Output with material alpha
    FragColor = vec4(result, diffuse.a);
}
