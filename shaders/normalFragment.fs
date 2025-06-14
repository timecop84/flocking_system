#version 150

/// @brief our output fragment colour
out vec4 fragColour;
in vec4 perNormalColour;

void main ()
{
  fragColour = perNormalColour;
}

