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

// 2. Struktura oswietlenia punktowego
struct LightParam
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Attenuation;
	vec3 Position; // Direction dla kierunkowego
};

// 3. Przykladowe swiatlo punktowe
LightParam myLight = LightParam
(
	vec3 (0.1 , 0.1 , 0.1), // ambient
	vec3 (1.0 , 1.0 , 1.0), // diffuse
	vec3 (1.0 , 1.0 , 1.0), // specular
	vec3 (1.0 , 0.0 , 0.0), // attenuation
	vec3 (2.0 , 3.0 , 1.0)  // position
);

// 4. Struktura materialu obiektu
struct MaterialParam
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

  	uniform vec3 MatAmbient;
	uniform vec3 MatDiffuse;
	uniform vec3 MatSpecular;
	uniform float MatShininess;
	
// 5. Przykladowy material
MaterialParam myMaterial = MaterialParam
(
	vec3 (0.2 , 0.2 , 0.2), // ambient
	vec3 (1.0 , 1.0 , 1.0), // diffuse
	vec3 (1.0 , 1.0 , 1.0), // specular
	32.0 // shininess
);


// 6. Nasza funkcja liczaca wspolczynnik oswietlenia punktowego
vec3 calculatePointLight(LightParam light)
{
	// Poczatkowa wartosc wspolczynnika oswietlenia
	vec3 lightCoeff = vec3(0.0);

	float LV = distance(light.Position, inPosition.xyz);
	float Latt = 1.0/( light.Attenuation.x + light.Attenuation.y * LV + light.Attenuation.z * LV * LV );

	// ambient
	vec3 ambientPart = light.Ambient * MatAmbient;

	// diffuse
	vec3 L = normalize( light.Position - inPosition.xyz );
	float diff = max( dot(L, inNormal) , 0 );
	vec3 diffusePart = Latt * ( diff * light.Diffuse * MatDiffuse );

	// specular
	vec3 E = normalize( cameraPosition - inPosition.xyz );
	vec3 H = normalize( L + E );
	float spec = pow(max( dot(H, inNormal) , 0 ), MatShininess);
	vec3 specularPart = Latt * ( spec * light.Specular * MatSpecular );


	lightCoeff = ambientPart + diffusePart + specularPart;
	return lightCoeff;
}
void main()
{
	// Kolor poczatkowy
	vec3 fragLightCoef = calculatePointLight(myLight);
	vec3 fragColor = fragLightCoef * texture( tex0, inUV ).rgb;

	outColor = vec4(fragColor, 1.0);
}
