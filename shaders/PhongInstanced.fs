#version 450 core

// Input from vertex shader
in vec3 FragPos;     // Position in view space
in vec3 Normal;      // Normal in view space
in vec4 InstanceColor; // Color from instance data

// Output
out vec4 fragColour;

// Lighting UBO  
layout(std140, binding = 3) uniform LightingBlock {
    vec3 lightPos;    // Light position in view space
    float pad1;       // Padding
    vec3 viewPos;     // View position (not used in view space)
    float pad2;       // Padding
    vec3 lightColor;  // Light color
    float shininess;  // Material shininess override
};

void main()
{
    // Normalize the normal (in case of interpolation issues)
    vec3 norm = normalize(Normal);
    
    // Calculate lighting in view space (simplified Phong)
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(-FragPos); // Camera is at origin in view space
    vec3 reflectDir = reflect(-lightDir, norm);
    
    // Use instance color as base material properties
    vec3 baseColor = InstanceColor.rgb;
    vec3 ambient = baseColor * 0.25; // Ambient
    vec3 diffuse = baseColor * 1.0;  // Diffuse
    vec3 specular = vec3(0.8, 0.8, 0.8); // Specular
    
    // Ambient component
    vec3 ambientColor = ambient * lightColor * 0.25;
    
    // Diffuse component
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diff * diffuse * lightColor;
    
    // Specular component
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularColor = spec * specular * lightColor;
    
    // Combine lighting components
    vec3 lighting = ambientColor + diffuseColor + specularColor;
    
    // Use instance color as the result
    vec3 result = lighting;
    
    // Apply slight gamma correction for better visibility
    result = pow(result, vec3(1.0/2.2));
    
    // Ensure minimum brightness for visibility
    result = max(result, vec3(0.10)); // Lower minimum brightness for darker shadows
    
    fragColour = vec4(result, InstanceColor.a);
}
