// Vertex shader
#version 330 core

// Dane pobrane z VAO
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inSizes;


// Predefiniowane dane wejsciowe
// nie odkomentowujemy!
/*
in int gl_VertexID;
in int gl_InstanceID;
*/

// Dane wyjsciowe (kolejny etap potoku)
// out vec4 gl_Position;


void main()
{

	// zmienna typu vec4
	vec4 newPosition = inPosition;
	gl_PointSize = inSizes.x;
	// finalna pozycja wierzcholka
	gl_Position = newPosition;

}