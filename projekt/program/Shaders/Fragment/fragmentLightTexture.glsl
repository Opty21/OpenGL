#version 150 core

// Zmienne wejsciowe z vertex shadera
in vec4 inPosition;
in vec2 inUV;
in vec3 inNormal;
// Zmienna wyjsciowa ostatecznego koloru
out vec4 outColor;
uniform sampler2D tex0;

// 1. Pozycja kamery do skladowej specular
uniform vec3 cameraPosition;

	//Mat Info
  	uniform vec3 MatAmbient;
	uniform vec3 MatDiffuse;
	uniform vec3 MatSpecular;
	uniform float MatShininess;
	
	//Light Info
  	uniform vec3 LightAmbient;
	uniform vec3 LightDiffuse;
	uniform vec3 LightSpecular;
	uniform vec3 LightShininess;
	uniform vec3 LightAttenuation;
	uniform vec3 LightPosition;

// 6. Nasza funkcja liczaca wspolczynnik oswietlenia punktowego
vec3 calculatePointLight()
{
	// Poczatkowa wartosc wspolczynnika oswietlenia
	vec3 lightCoeff = vec3(0.0);

	float LV = distance(LightPosition, inPosition.xyz);
	float Latt = 1.0/( LightAttenuation.x + LightAttenuation.y * LV + LightAttenuation.z * LV * LV );

	// ambient
	vec3 ambientPart = LightAmbient * MatAmbient;

	// diffuse
	vec3 L = normalize( LightPosition - inPosition.xyz );
	float diff = max( dot(L, inNormal) , 0 );
	vec3 diffusePart = Latt * ( diff * LightDiffuse * MatDiffuse );

	// specular
	vec3 E = normalize( cameraPosition - inPosition.xyz );
	vec3 H = normalize( L + E );
	float spec = pow(max( dot(H, inNormal) , 0 ), MatShininess);
	vec3 specularPart = Latt * ( spec * LightSpecular * MatSpecular );


	lightCoeff = ambientPart + diffusePart + specularPart;
	return lightCoeff;
}
void main()
{
	// Kolor poczatkowy
	vec3 fragLightCoef = calculatePointLight();
	vec3 fragColor = fragLightCoef * texture( tex0, inUV ).rgb;

	outColor = vec4(fragColor, 1.0);
}
