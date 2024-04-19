#version 150

in vec4 inoutPos;
out vec4 outColor;
in vec3 fragLightCoef;

uniform int Mode;

void main()
{

	outColor = vec4(0,0,sin(inoutPos.x)+0.8, 1.0);
}
