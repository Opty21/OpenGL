// Fragment shader
#version 330 core


// Predefiniowane dane wejsciowe
/*
in  vec4 gl_FragCoord;
in  int  gl_PrimitiveID;
*/

// Dane wyjsciowe
out vec4 outColor;

in vec3 myColor;


uniform float Jasnosc;

void main()
{

	// szerokosc gl_FragCoord.x
	// wysokosc gl_FragCoord.y

    vec3 newColor = myColor
        * vec3(Jasnosc, Jasnosc, Jasnosc);

    outColor = vec4(newColor, 1.0);


}
