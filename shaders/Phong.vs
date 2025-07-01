#version 330 core

layout(location = 0) in vec3 inVert;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform mat3 normalMatrix;
uniform vec3 viewerPos;
uniform bool Normalize;

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

out vec3 fragmentNormal;
out vec3 lightDir;
out vec3 halfVector;
out vec3 eyeDirection;
out vec3 vPosition;

void main() {
    gl_Position = MVP * vec4(inVert, 1.0);
    
    fragmentNormal = normalMatrix * inNormal;
    if (Normalize) {
        fragmentNormal = normalize(fragmentNormal);
    }
    
    vec4 worldPosition = M * vec4(inVert, 1.0);
    eyeDirection = normalize(viewerPos - worldPosition.xyz);
    
    vec4 eyePosition = MV * vec4(inVert, 1.0);
    vPosition = eyePosition.xyz / eyePosition.w;
    
    lightDir = normalize(light.position.xyz - eyePosition.xyz);
    halfVector = normalize(eyeDirection + lightDir);
}
