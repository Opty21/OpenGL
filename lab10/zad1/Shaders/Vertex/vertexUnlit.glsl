#version 330

// Wspolrzedne wierzcholkow
layout( location = 0 ) in vec4 inPosition;
// -----------------------------------------------------
// Krok 2. Odebranie wsp�rz�dnych UV w vertex shaderze
//         i przekazanie ich do fragment shadera.
// -----------------------------------------------------
layout( location = 1 ) in vec2 inUV;
layout( location = 3 ) in mat4 matModelInst;

// Macierz rzutowania
uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;
uniform int instantiated;

out vec4 inoutPos;
out vec3 inoutColor;
out vec2 inoutUV;


void main()
{
	if(instantiated == 1){
		gl_Position = matProj * matView * matModelInst * inPosition;
	} else{
		gl_Position = matProj * matView * matModel * inPosition;
	}
	inoutPos = inPosition;
	inoutUV = inUV;
}
