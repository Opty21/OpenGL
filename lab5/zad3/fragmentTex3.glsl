#version 150 core

in vec2 inoutUV;
out vec4 outColor;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float val;
void main()
{
	vec4 texColor = texture( tex0, inoutUV );
	if(val < 1){
		 texColor = mix(texture(tex2, inoutUV), texture(tex1, inoutUV), val);

	} else{
		 texColor = mix(texture(tex1, inoutUV), texture(tex0, inoutUV), val-1);
	}
	if(texColor.a <0.2) discard;

	outColor = texColor;
}
