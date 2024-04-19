#version 330
layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec3 inNormal;
layout( location = 3 ) in mat4 matModelInst;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;
uniform int instantiated;

out vec4 ourPos;
out vec3 ourNormal;



void main()
{
	if(instantiated == 1){
			gl_Position = matProj * matView * matModelInst * inPosition;
			ourPos = matModelInst * inPosition;
			ourNormal = inNormal;
	}
	else{
	gl_Position = matProj * matView * matModel * inPosition;
	ourPos = matModel * inPosition;
	ourNormal = inNormal;
	}


}
