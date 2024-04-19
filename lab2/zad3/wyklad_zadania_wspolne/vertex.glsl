#version 330 core

layout (location = 0) in vec4 inPosition;

void main()
{

    gl_Position = vec4(-inPosition[1], -inPosition[0], inPosition[2], inPosition[3]);

}
