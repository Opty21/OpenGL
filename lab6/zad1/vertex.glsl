#version 330 core

// Atrybuty wierzcholkow
layout( location = 0 ) in vec4 inPositionV;
layout( location = 1 ) in vec2 inUVV;
layout( location = 2 ) in vec3 inNormalV;

// Macierze rzutowania, widoku i modelu
uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;
uniform mat3 matNormal;

out vec2 inUV;
out vec4 inPosition;
out vec3 inNormal;
// ---------------------------------------
// NOWE: Oswietlenie:
// ---------------------------------------

// ---------------------------------------------------
// Zmienne wyjsciowe do fragment shadera


out vec3 fragPos;


void main()
{
	inUV = inUVV;
	inPosition = inPositionV;
	inNormal = matNormal * inNormalV;



	gl_Position = matProj * matView * matModel * inPosition;
	fragPos = (matModel * inPosition).xyz;
}
