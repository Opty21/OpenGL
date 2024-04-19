// Reference: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows
#include "Classes/global.h"

#ifndef __SHADOW_POINT_LIGHT
#define __SHADOW_POINT_LIGHT

// ---------------------------------------
class CShadowPointLight
{

public:

	// Polozenie swiatla
	glm::vec3 Position;

	// Identyfikatory obiektow
	GLuint idTexture;		// tekstura
	GLuint idProgram;		// program generujacy mape cieni
	GLuint idFrameBuffer;	// ramka buforu (rendering pozaekranowy)

	// Rozmiar mapy cieni
	int Shadow_Width = 1024;
	int Shadow_Height = 1024;

	// Bryla obcinania (frustum)
	float frustumNear = 1.0f;
	float frustumFar = 200.0f;
	// macierz projection oraz view
	// do renderingu kazdej z szesciu scian
	glm::mat4 matProj;
	glm::mat4 matPVArray[6];


	// ----------------------------------------------------------
	// Etap 1. Inicjalizacja calego procesu z podaniem pozycji oswietlenia
	void Init(glm::vec3 _pos);

	// Aktualizacja polozenia oswietlenia i macierzy rzutowania
	// potrzebna podczas inicjalizacji oraz zawsze wtedy, kiedy
	// dokonujemy zmiany polozenia oswietlenia
	void UpdatePV();

	// Etap 2: Generowanie mapy cieni
	// pomiedzy tymi funkcjami rysujemy to co ma rzucac cien
	// i byc uwzglednione w mapie cieni
	// UWAGA! generowanie obiektow powinno odbywac sie za pomoca
	// programu idProgram z tego obiektu.
	void GenBegin(void);
	void GenEnd(void);


	// Etap 3: Podczas finalnego renderingu na ekran
	// Wysylanie do programu _prog wszystkich potrzebnych uniformow
	// Ustawianie tekstury mapy cieni na slot _texSlot
	void SendUniform(GLuint _prog, GLuint _texSlot, glm::vec3 _camPos);

};

// -------------------------------------------------------
void CShadowPointLight::SendUniform(GLuint _prog, GLuint _texSlot, glm::vec3 _camPos)
{

	// Shadow map textura na slot _texSlot
	glActiveTexture(GL_TEXTURE0 + _texSlot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, idTexture);
	glUniform1i(glGetUniformLocation( _prog, "tex_shadowCubeMap" ), _texSlot);

	// Parametry oswietlenia
	glUniform3fv( glGetUniformLocation( _prog, "LightPosition" ), 1, glm::value_ptr(Position) );
	glUniform1f( glGetUniformLocation( _prog, "FarPlane" ), frustumFar );

	// Inne : pozycja kamery
	glUniform3fv( glGetUniformLocation( _prog, "CameraPosition" ), 1, glm::value_ptr(_camPos) );


}

// -------------------------------------------------------
void CShadowPointLight::GenBegin(void)
{

	glViewport(0, 0, Shadow_Width, Shadow_Height);
	glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	// AKTYWUJEMY program
	glUseProgram( idProgram );
	_currentProgramId = idProgram;

	// Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
}


// -------------------------------------------------------
void CShadowPointLight::GenEnd(void)
{

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	// WYLACZAMY program
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// -------------------------------------------------------
// Aktualizacja tablicy 6 macierzy rzutowania
void CShadowPointLight::UpdatePV()
{
	// Obliczanie nowych macierzy rzutowania
	matPVArray[0] = matProj * glm::lookAt(Position, Position + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0));
	matPVArray[1] = matProj * glm::lookAt(Position, Position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0));
	matPVArray[2] = matProj * glm::lookAt(Position, Position + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	matPVArray[3] = matProj * glm::lookAt(Position, Position + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0));
	matPVArray[4] = matProj * glm::lookAt(Position, Position + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0));
	matPVArray[5] = matProj * glm::lookAt(Position, Position + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0));

	// Wyslanie do programu
	glUseProgram( idProgram );

	for (int i = 0; i < 6; ++i)
	{
		char name[100];
		sprintf(name, "matPVArray[%i]", i);
		glUniformMatrix4fv( glGetUniformLocation( idProgram, name ), 1, GL_FALSE, glm::value_ptr(matPVArray[i]));
	}

	glUniform3fv( glGetUniformLocation( idProgram, "LightPosition" ), 1, glm::value_ptr(Position) );
	glUniform1f( glGetUniformLocation( idProgram, "FarPlane" ), frustumFar );

}


// -------------------------------------------------------
void CShadowPointLight::Init(glm::vec3 _pos)
{
	// 1. Pozycja zrodla swiatla
	this->Position = _pos;
	// 2. Textura do depthMapy
	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, idTexture);

	for (int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, Shadow_Width, Shadow_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// 3. Framebuffer z buforem glebokosci
	glGenFramebuffers(1, &idFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, idFrameBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, idTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 4. Program Tworzenie potoku OpenGL
	idProgram = glCreateProgram();
	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "Shaders/ShadowPointLight_DepthMap.vert"));
	glAttachShader( idProgram, LoadShader(GL_GEOMETRY_SHADER, "Shaders/ShadowPointLight_DepthMap.geom"));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "Shaders/ShadowPointLight_DepthMap.frag"));
	LinkAndValidateProgram( idProgram );


	// 5. Ustawienia kamery cienia
	matProj = glm::perspective(glm::radians(90.0f), Shadow_Width/(float)Shadow_Height, frustumNear, frustumFar);
	// aktualizacja macierzu rzutowania
	UpdatePV();

}



#endif
