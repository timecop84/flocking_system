#version 450 core

// Input from vertex shader
in vec3 FragPos;     // Position in view space
in vec3 Normal;      // Normal in view space
in vec4 InstanceColor; // Color from instance data

// Output
out vec4 fragColour;

// Uniform Buffer Object for material
layout(std140, binding = 1) uniform MaterialBlock {
    vec4 material_ambient;
    vec4 material_diffuse;
    vec4 material_specular;
    float material_shininess;
};

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
    
    // Ambient component (use material ambient with reduced brightness)
    vec3 ambient = material_ambient.rgb * lightColor * 0.25; // Reduced ambient for darker appearance
    
    // Diffuse component (use material diffuse)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material_diffuse.rgb * lightColor;
    
    // Specular component (use material specular)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * material_specular.rgb * lightColor;
    
    // Combine lighting components
    vec3 lighting = ambient + diffuse + specular;
    
    // Mix with instance color for variety (instead of multiplying)
    vec3 result = mix(lighting, InstanceColor.rgb, 0.1); // 10% instance color tinting
    
    // Apply slight gamma correction for better visibility
    result = pow(result, vec3(1.0/2.2));
    
    // Ensure minimum brightness for visibility
    result = max(result, vec3(0.10)); // Lower minimum brightness for darker shadows
    
    fragColour = vec4(result, InstanceColor.a);
}
