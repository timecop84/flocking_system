#version 450 core

// Inputs from vertex shader (in view space, like fixed function)
in vec3 FragPos;     // Fragment position in view space
in vec3 Normal;      // Surface normal in view space

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
    vec3 lightPos;   // Light position in view space (same as PhongInstanced)
    float pad1;      // Padding
    vec3 viewPos;    // Camera position (redundant with viewerPos above)
    float pad2;      // Padding
    vec3 lightColor; // Light color
    float lightShininess; // Light-specific shininess (unused)
};

void main()
{
    // Normalize the normal (in case of interpolation issues)
    vec3 norm = normalize(Normal);
    
    // Calculate lighting in view space (light position already in view space)
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(-FragPos); // Camera is at origin in view space
    vec3 reflectDir = reflect(-lightDir, norm);
    
    // Ambient component (reduced for more natural lighting)
    vec3 ambientColor = ambient.rgb * lightColor * 0.10; // Further reduced ambient for darker appearance
    
    // Diffuse component (use material diffuse)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diff * diffuse.rgb * lightColor;
    
    // Specular component (use material specular)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularColor = spec * specular.rgb * lightColor;
    
    // Combine lighting components
    vec3 lighting = ambientColor + diffuseColor + specularColor;
    
    // Apply slight gamma correction for better visibility
    vec3 result = pow(lighting, vec3(1.0/2.2));
    
    // Ensure minimum brightness for visibility
    result = max(result, vec3(0.10)); // Lower minimum brightness for darker shadows
    
    FragColor = vec4(result, diffuse.a);
}
