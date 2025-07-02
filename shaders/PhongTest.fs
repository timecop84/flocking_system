#version 330 core

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec3 v_LightDir;
in vec3 v_ViewDir;

out vec4 fragColour;

uniform vec3 u_LightColor;
uniform vec3 u_AmbientColor;

void main() {
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(v_LightDir);
    vec3 viewDir = normalize(v_ViewDir);
    
    // Simple Lambertian shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Blinn-Phong specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    
    vec3 ambient = u_AmbientColor;
    vec3 diffuse = diff * u_LightColor;
    vec3 specular = spec * u_LightColor * 0.5;
    
    vec3 result = ambient + diffuse + specular;
    fragColour = vec4(result, 1.0);
}
