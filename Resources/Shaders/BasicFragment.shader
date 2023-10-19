#version 330 core

out vec4 colour;
in vec3 vertexColour;
in float scale;

void main()
{
    colour = vec4(vertexColour.x, vertexColour.y, vertexColour.z, 1.0f);
}