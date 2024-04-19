// Fragment shader
#version 330 core


// Predefiniowane dane wejsciowe
/*
in  vec4 gl_FragCoord;
in  int  gl_PrimitiveID;
*/

// Dane wyjsciowe
out vec4 outColor;

in float myColor;


void main()
{

	// szerokosc gl_FragCoord.x
	// wysokosc gl_FragCoord.y

    outColor = vec4(0,myColor,0, 1.0);


}
