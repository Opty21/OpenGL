#version 150

in vec4 inoutPos;
out vec4 outColor;

uniform int Mode;

void main()
{
	float odl = sqrt(inoutPos.x*inoutPos.x + inoutPos.y*inoutPos.y + inoutPos.z*inoutPos.z);

	outColor = vec4(139.0,69.0 * odl/1.8,19.0, 1.0)/255 ;
}
