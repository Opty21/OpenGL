#version 330 core

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec2 inUV;

uniform mat4 matPV;

out vec2 inoutUV;


void main()
{
	gl_Position = matPV * inPosition;

	inoutUV = inUV;
}
