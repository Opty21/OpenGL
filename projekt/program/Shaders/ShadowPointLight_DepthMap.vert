#version 330 core
layout (location = 0) in vec4 inPosition;
layout( location = 3 ) in mat4 matModelInst;

uniform mat4 matModel;

uniform mat4 matProj;
uniform mat4 matView;

out vec4 ourPos;
out vec3 ourNormal;
uniform int instantiated;
out vec4 fragPos;



void main()
{
	if(instantiated == 1){
		gl_Position = matProj * matView * matModelInst * inPosition;
		ourPos = matModelInst * inPosition;
		fragPos = (matModelInst * inPosition);

	} else{
		gl_Position = matProj * matView * matModel * inPosition;
		ourPos = matModel * inPosition;
		fragPos = (matModel * inPosition);

	}
}
