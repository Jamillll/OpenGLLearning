#version 330 core

layout(location = 0) in vec2 position;

layout(location = 1) in vec3 inputColour;
out vec3 vertexColour;

uniform float u_Scale;
out float scale;

void main()
{
    gl_Position = vec4(position, 1.0f, 1.0f);
    vertexColour = inputColour;
    scale = u_Scale;
}