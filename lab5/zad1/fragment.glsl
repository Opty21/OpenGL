#version 150

in vec4 inoutPos;
in vec3 inoutColor;
in vec2 inoutUV;

// Sampler tekstury
uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D tex3;


out vec4 outColor;

void main()
{
	// -----------------------------------------------------
	// Krok 7. Odczytanie danych tekstury za pomoc� uchwytu
	//         w fragment shaderze.
	// -----------------------------------------------------
	vec4 texColor = mix(texture(tex, inoutUV), texture(tex2, inoutUV), 0.6);
	texColor = mix(texture(tex3,inoutUV),texColor,0.9);

	outColor = vec4(texColor.rgb, 1.0);
}
