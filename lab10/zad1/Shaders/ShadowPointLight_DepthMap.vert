#version 330 core
layout (location = 0) in vec4 inPosition;
uniform mat4 matModel;

uniform mat4 matProj;
uniform mat4 matView;

out vec4 ourPos;
out vec3 ourNormal;



void main()
{
	gl_Position = matModel *  inPosition;
	ourPos = matModel * inPosition;
}
