#version 330 core

uniform vec4 Colour;

out vec4 fragColour;

void main() {
    fragColour = Colour;
}
