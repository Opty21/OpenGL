// ---------------------------------------------------------------------------
// https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows
// ---------------------------------------------------------------------------

#version 150 core

in vec4 ourPos;
in vec3 ourNormal;

out vec4 outColor;

// ---------------------------------------------------------------------------
// Zmienne i parametry oswietlenia

uniform samplerCube tex_shadowCubeMap;

uniform float FarPlane;
uniform vec3  CameraPosition;
uniform vec3  LightPosition;

// -----------------------
// Oswietlenie
struct LightParam
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    vec3 Position;
    float Attenuation;
};


// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);


// ---------------------------------------------------------------------------
// Reference: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows
float Calculate_ShadowPoint(vec3 lightPosition, vec3 fragPos, float FarPlane)
{
	// get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPosition;

	// now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    // use the light to fragment vector to sample from the depth map
    float closestDepth = texture(tex_shadowCubeMap, fragToLight).r * FarPlane;


	float bias = 0.001;
	float shadow = 0.0;


//  simple test
  shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	return shadow;

	// filtering
    int samples = 20;
    float viewDistance = length(CameraPosition - fragPos.xyz);
    float diskRadius = (1.0 + (viewDistance / FarPlane)) / 25.0;

    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(tex_shadowCubeMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= FarPlane;   // undo mapping [0;1]

        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    shadow /= float(samples);

    return shadow;
}



// ---------------------------------------------------------------------------
vec3 Calculate_PointLight(LightParam light, vec3 cameraPos, vec3 fragPos, vec3 fragNormal)
{

	// Distance light
	float Light_dist = length(light.Position - fragPos);
	float Attenuation = 1.0 / (1.0 + light.Attenuation * (Light_dist));

	// ---------------------------
	// Ambient
	// vec4 ambientPart = myLight.Ambient * Attenuation;

	// ---------------------------
	// Diffuse

	// Obliczenie wektora (swiatlo - wierzcholek)
	// czyli kierunku padania swiatla na wierzcholek
	vec3 lightDir = normalize(light.Position - fragPos);

	// obliczenie kata pomiedzy wektorem lightDir oraz wektorem normalnym
	// wartosc kata okresla pod jakim katem padaja promienie
	float lightCoeff = max(dot(fragNormal, lightDir), 0.0);
	vec3 diffusePart = lightCoeff * light.Diffuse * Attenuation;


	// ------------------
	// Specular
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3  reflectDir = reflect(-lightDir, fragNormal);
	// obliczanie wspolczynnika specular z parametrem shininess
	float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3  specularPart = specularCoeff * light.Specular * Attenuation;

	// -----------------
	// Ostateczny
	return (diffusePart + specularPart);
}



// ---------------------------------------------------------------------------
void main()
{

	// Przykladowe parametry oswietlenia
	LightParam myLight = LightParam(
		vec3(0.2, 0.2, 0.2),
		vec3(0.8, 0.8, 0.8),
		vec3(0.5, 0.5, 0.5),
		LightPosition,
		0.01
	);


	// Kolor wyjsciowy
	vec3  fragColor = vec3(0.1, 1.0, 0.1);

	// Oswietlenie kierunkowe
	vec3  lightPart = Calculate_PointLight(myLight, CameraPosition, ourPos.xyz, ourNormal);

	// Cienie
	float shadowPart = Calculate_ShadowPoint(myLight.Position, ourPos.xyz, FarPlane);

	// Kolor finalny
	vec3  finalColor = (myLight.Ambient + (1 - shadowPart) * lightPart) * fragColor;

	outColor = vec4(finalColor, 1.0);

}
