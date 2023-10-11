#version 330 core

out vec4 colour;
in vec3 vertexColour;

void main()
{
    colour = vec4(vertexColour, 1.0f);
}