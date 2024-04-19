#version 330

// Wspolrzedne wierzcholkow
layout( location = 0 ) in vec4 inPosition;

// Macierz rzutowania
uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

out vec4 inoutPos;

void main()
{
	gl_Position = matProj * matView * matModel * inPosition;
	inoutPos = inPosition;
}
