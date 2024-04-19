// Fragment shader
#version 330 core


// Predefiniowane dane wejsciowe
/*
in  vec4 gl_FragCoord;
in  int  gl_PrimitiveID;
*/

// Dane wyjsciowe
out vec4 outColor;


void main()
{

		outColor = vec4(gl_FragCoord.x/800, 0.0, 1.0, 1.0);
		//outColor = vec4(1, 1.0, gl_FragCoord.x/800, 1.0);
	


}