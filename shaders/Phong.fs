#version 330 core

// Inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;

// Output color
out vec4 fragColour;

// UBO for material properties (must match vertex shader)
layout(std140) uniform MaterialBlock {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float padding[3];   // Padding for alignment
} material;

// UBO for lighting properties (must match vertex shader)
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

// Additional uniform for camera position
layout(std140) uniform MatrixBlock {
    mat4 MVP;
    mat4 MV;
    mat4 M;
    mat3 normalMatrix;
    vec3 viewerPos;
    float shouldNormalize;
};

void main() {
    // Normalize the interpolated normal from vertex shader
    vec3 norm = normalize(Normal);
    
    // Calculate lighting vectors
    vec3 lightDir = normalize(light.position.xyz - FragPos);
    vec3 viewDir = normalize(viewerPos - FragPos);
    
    // Ambient component
    vec3 ambient = light.ambient.rgb * material.ambient.rgb;
    
    // Diffuse component (Lambertian)
    float NdotL = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse.rgb * material.diffuse.rgb * NdotL;
    
    // Specular component (Blinn-Phong)
    vec3 specular = vec3(0.0);
    if (NdotL > 0.0) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float NdotH = max(dot(norm, halfwayDir), 0.0);
        specular = light.specular.rgb * material.specular.rgb * pow(NdotH, material.shininess);
    }
    
    // Final color
    vec3 finalColor = ambient + diffuse + specular;
    fragColour = vec4(finalColor, 1.0);
}
