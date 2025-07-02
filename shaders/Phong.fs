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
    vec3 lightPos;   // Light position in world space
    float pad1;      // Padding
    vec3 viewPos;    // Camera position (redundant with viewerPos above)
    float pad2;      // Padding
    vec3 lightColor; // Light color
    float lightShininess; // Light-specific shininess (unused)
};

void main()
{
    // Since we're in view space, the view direction is simply (0,0,1)
    vec3 viewDir = vec3(0.0, 0.0, 1.0);
    
    // Transform light position to view space (like fixed function)
    vec3 lightPosView = (MV * vec4(lightPos, 1.0)).xyz;
    vec3 lightDirView = normalize(lightPosView - FragPos);
    
    // Ambient - increased for better base illumination
    vec3 ambientColor = ambient.rgb * lightColor * 0.4;  // Increased ambient
    
    // Diffuse - much brighter with gentler falloff
    float diff = max(dot(Normal, lightDirView), 0.0);
    diff = diff * 0.8 + 0.2;  // Prevent complete darkness
    vec3 diffuseColor = diffuse.rgb * lightColor * diff * 1.5;  // Increased diffuse multiplier
    
    // Specular - keep the sharp highlights
    vec3 reflectDir = reflect(-lightDirView, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess * 1.5);  // Reduced shininess multiplier
    vec3 specularColor = specular.rgb * lightColor * spec * 3.0;
    
    // Much lighter attenuation
    float distanceToLight = length(lightPosView - FragPos);
    float attenuation = 1.0 / (1.0 + 0.005 * distanceToLight + 0.0001 * distanceToLight * distanceToLight);
    
    // Final color with increased brightness
    vec3 result = ambientColor + (diffuseColor + specularColor) * attenuation;
    
    // Optional slight boost to overall brightness
    result *= 1.2;
    
    // Output without gamma correction (fixed function doesn't use it)
    FragColor = vec4(result, diffuse.a);
}
