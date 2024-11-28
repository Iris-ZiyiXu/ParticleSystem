#version 330 core

layout(location=0)in vec3 position; 

out vec4 theColor;

uniform mat4 modelTransformMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 color;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelTransformMatrix * vec4(position, 1.0f);

    theColor = color;
}
