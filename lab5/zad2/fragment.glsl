#version 150 core

in vec2 inoutUV;
out vec4 outColor;

uniform sampler2D tex0;

void main()
{
	vec4 texColor = texture( tex0, inoutUV );
	if(texColor.r > 0.5 && texColor.b <0.5) discard;
	outColor = texColor;
}
