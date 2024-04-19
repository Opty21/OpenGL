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


uniform int Mode;

void main()
{

	// szerokosc gl_FragCoord.x
	// wysokosc gl_FragCoord.y
    switch (Mode)
      {
         case 0:
            outColor = vec4(13, 35, 87, 1);
            break;
         case 1:
            outColor = vec4(0, 112, 128, 1);
            break;
         case 2:
            outColor = vec4(0, 214, 186, 1);
            break; 
         case 3:
            outColor = vec4(255, 250, 214, 1);
            break;   
         default:
            outColor = vec4(1.0,1.0,1.0, 1.0);
      }
      
    outColor = outColor/255;

}
