#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "obj_loader.hpp"
#include "shader_stuff.hpp"


GLuint idProgram;
GLuint vBuffer_pos;
GLuint vBuffer_uv;
GLuint vBuffer_normal;
GLuint vArray;
GLuint idTexture;


glm::mat4x4 matProj;
glm::mat4x4 matView;
glm::mat4x4 matPV;


std::vector<glm::vec3> OBJ_vertices;
std::vector<glm::vec2> OBJ_uvs;
std::vector<glm::vec3> OBJ_normals;


// ---------------------------------------
void DisplayScene()
{

	// Czyszczenie ramki
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Geometria sceny
	matView = glm::mat4x4( 1.0 );
	matView = glm::translate( matView, glm::vec3( _scene_translate_x, _scene_translate_y, _scene_translate_z ) );
	matView = glm::rotate( matView, _scene_rotate_x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	matView = glm::rotate( matView, _scene_rotate_y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	matPV = matProj * matView;


	glUseProgram(idProgram);

		glUniformMatrix4fv( glGetUniformLocation( idProgram, "matPV" ), 1, GL_FALSE, glm::value_ptr(matPV) );

		// Wlaczanie/aktywowanie tekstur
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glUniform1i(glGetUniformLocation(idProgram, "tex0"), 0);

		// Generowanie obiektow na ekranie
		glBindVertexArray( vArray );
		glDrawArrays( GL_TRIANGLES, 0, OBJ_vertices.size() );
		glBindVertexArray( 0 );

	glUseProgram(0);

	glutSwapBuffers();
}


// ---------------------------------------
void Reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
	matProj = glm::perspectiveFov(glm::radians(60.0f), (float)width, (float)height, 0.1f, 100.f );
}

// ---------------------------------------------------
void Initialize()
{
	// Glowne ustawienia OpenGL
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glEnable( GL_DEPTH_TEST );

		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// Ladowanie pliku OBJ
	if (!loadOBJ("coolGuy.obj", OBJ_vertices, OBJ_uvs, OBJ_normals))
	{
		printf("Not loaded!\n");
		exit(1);
	}


	// -------------------------------------------------------
	// Wczytanie pliku graficznego
	// -------------------------------------------------------

	int tex_width, tex_height, n;
	unsigned char *tex_data;
	stbi_set_flip_vertically_on_load(true); // tylko raz aby ...

	tex_data = stbi_load("paper.png", &tex_width, &tex_height, &n, 0);
	if (tex_data == NULL) {
		printf("Image can't be loaded!\n");
		exit(1);
	}

	// -------------------------------------------------------
	// Tworzenie obiektu tekstury
	// -------------------------------------------------------

	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	// -------------------------------------------------------
	// Tworzenie potoku i VAO
	// -------------------------------------------------------

	// Potok
	idProgram = glCreateProgram();
	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));
	LinkAndValidateProgram( idProgram );

	// VAO
	glGenVertexArrays( 1, &vArray );
	glBindVertexArray( vArray );

	// Wspolrzedne wierzchokow
	glGenBuffers( 1, &vBuffer_pos );
	glBindBuffer( GL_ARRAY_BUFFER, vBuffer_pos );
	glBufferData( GL_ARRAY_BUFFER, OBJ_vertices.size() * sizeof(glm::vec3), &OBJ_vertices[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	// Wspolrzedne textury UV
	glGenBuffers( 1, &vBuffer_uv );
	glBindBuffer( GL_ARRAY_BUFFER, vBuffer_uv );
	glBufferData( GL_ARRAY_BUFFER, OBJ_uvs.size() * sizeof(glm::vec2), &OBJ_uvs[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );




}



// ---------------------------------------------------
int main( int argc, char *argv[] )
{
	// GLUT
	glutInit( &argc, argv );

	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextVersion( 4, 0 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow( "OpenGL" );

	// GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		printf("GLEW Error\n");
		exit(1);
	}

	// OpenGL
	if( !GLEW_VERSION_3_2 )
	{
		printf("Brak OpenGL 3.2!\n");
		exit(1);
	}


	Initialize();
	glutDisplayFunc( DisplayScene );
	glutReshapeFunc( Reshape );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutMouseWheelFunc( MouseWheel );
	glutKeyboardFunc( Keyboard );
	glutSpecialFunc( SpecialKeys );

	glutMainLoop();

	// Cleaning();
	glDeleteProgram( idProgram );
	glDeleteBuffers( 1, &vBuffer_pos );
	glDeleteBuffers( 1, &vBuffer_uv );
	glDeleteBuffers( 1, &vBuffer_normal );
	glDeleteVertexArrays( 1, &vArray );

	return 0;
}
