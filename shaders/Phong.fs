#version 330 core

in vec3 fragmentNormal;
in vec3 lightDir;
in vec3 halfVector;
in vec3 eyeDirection;
in vec3 vPosition;

out vec4 fragColour;

struct Materials {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct Lights {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    float spotCosCutoff;
};

uniform Materials material;
uniform Lights light;

vec4 pointLight() {
    vec3 N = normalize(fragmentNormal);
    vec3 L = normalize(lightDir);
    vec3 E = normalize(eyeDirection);
    vec3 H = normalize(halfVector);
    
    float distance = length(light.position.xyz - vPosition);
    float attenuation = 1.0 / (light.constantAttenuation + 
                              light.linearAttenuation * distance + 
                              light.quadraticAttenuation * distance * distance);
    
    vec4 ambient = light.ambient * material.ambient;
    
    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = light.diffuse * material.diffuse * NdotL;
    
    vec4 specular = vec4(0.0);
    if (NdotL > 0.0) {
        float NdotH = max(dot(N, H), 0.0);
        specular = light.specular * material.specular * pow(NdotH, material.shininess);
    }
    
    return ambient + attenuation * (diffuse + specular);
}

void main() {
    fragColour = pointLight();
}
