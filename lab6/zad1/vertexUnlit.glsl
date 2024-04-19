#version 330

// Wspolrzedne wierzcholkow
layout( location = 0 ) in vec4 inPosition;
// -----------------------------------------------------
// Krok 2. Odebranie wsp�rz�dnych UV w vertex shaderze
//         i przekazanie ich do fragment shadera.
// -----------------------------------------------------
layout( location = 1 ) in vec2 inUV;

// Macierz rzutowania
uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

out vec4 inoutPos;
out vec3 inoutColor;
out vec2 inoutUV;


void main()
{
	gl_Position = matProj * matView * matModel * inPosition;
	inoutPos = inPosition;
	inoutUV = inUV;
}
