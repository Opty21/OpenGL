#version 150

in vec4 inoutPos;
out vec4 outColor;
in vec3 fragLightCoef;
in vec4 inPosition;
in vec2 inUV;
in vec3 inNormal;
uniform int Mode;

void main()
{
	float odl = sqrt(inoutPos.x*inoutPos.x + inoutPos.y*inoutPos.y + inoutPos.z*inoutPos.z);

	outColor = vec4(vec3(139.0,69.0 * odl/1.8,19.0)/255 * fragLightCoef,1) ;
}
