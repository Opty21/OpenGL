#version 330 core

// Atrybuty wierzcholkow
layout( location = 0 ) in vec4 inPositionV;
layout( location = 1 ) in vec2 inUVV;
layout( location = 2 ) in vec3 inNormalV;
layout( location = 3 ) in mat4 matModelInst;
// Macierze rzutowania, widoku i modelu
uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;
uniform mat3 matNormal;
uniform int instantiated;
out vec2 inUV;
out vec4 inPosition;
out vec3 inNormal;
// ---------------------------------------
// NOWE: Oswietlenie:
// ---------------------------------------

// ---------------------------------------------------
// Zmienne wyjsciowe do fragment shadera

//in int gl_InstanceID;
out vec3 fragPos;


void main()
{
	inUV = inUVV;
	inPosition = inPositionV;
	inNormal = matNormal * inNormalV;
	//inPosition.x += gl_InstanceID/100;
	if(instantiated == 1){
		gl_Position = matProj * matView * matModelInst * inPosition;
	} else{
	gl_Position = matProj * matView * matModel * inPosition;
	}

	fragPos = (matModel * inPosition).xyz;
}
