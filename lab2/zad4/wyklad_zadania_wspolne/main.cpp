#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "house.h"
#include "shader_stuff.h"


// ---------------------------------------
// Identyfikatory obiektow OpenGL

GLuint idProgram;	// programu
GLuint idProgram2;	// programu
GLuint idVAO;		// tablic wierzcholkow
GLuint idVBO_coord; // bufora na wspolrzedne
GLuint idVBO_colors; // bufora na kolory

// ---------------------------------------
// Wspolrzedne wierzchokow

float Bright = 0.0;
float width = 800;
float height = 600;
float wdd = 800;
// ---------------------------------------
void DisplayScene()
{

	// ---------------------------
	// Etap (5) rendering
	// ---------------------------
	glClear( GL_COLOR_BUFFER_BIT );


	// Wlaczenie potoku
	
	//viewport 1
	glViewport(0, 0, width / 2, height / 2);
	glUseProgram( idProgram );
    // Ustawmy wartosc zmiennej jednorodnej
	glUniform1i(glGetUniformLocation(idProgram, "Mode"), 0);
    glBindVertexArray( idVAO );
	glDrawArrays( GL_TRIANGLES, 0, NUMBER_OF_VERTICES );
    glBindVertexArray( 0 );
	glUseProgram( 0 );

	//viewport 2
	glViewport(0, height / 2, width / 2, height / 2);
	glUseProgram( idProgram );
    // Ustawmy wartosc zmiennej jednorodnej
	glUniform1i(glGetUniformLocation(idProgram, "Mode"), 1);
    glBindVertexArray( idVAO );
	glDrawArrays( GL_TRIANGLES, 0, NUMBER_OF_VERTICES );
    glBindVertexArray( 0 );
	glUseProgram( 0 );

	//viewport 3
	glViewport(width / 2, 0, width / 2, height / 2);
	glUseProgram( idProgram );
    // Ustawmy wartosc zmiennej jednorodnej
	glUniform1i(glGetUniformLocation(idProgram, "Mode"), 2);
    glBindVertexArray( idVAO );
	glDrawArrays( GL_TRIANGLES, 0, NUMBER_OF_VERTICES );
    glBindVertexArray( 0 );
	glUseProgram( 0 );

	//viewport 4
	glViewport(width / 2, height / 2, width / 2, height / 2);
	glUseProgram( idProgram );
    // Ustawmy wartosc zmiennej jednorodnej
	glUniform1i(glGetUniformLocation(idProgram, "Mode"), 3);
    glBindVertexArray( idVAO );
	glDrawArrays( GL_TRIANGLES, 0, NUMBER_OF_VERTICES );
    glBindVertexArray( 0 );
	glUseProgram( 0 );

	glutSwapBuffers();
}

// ---------------------------------------
void Initialize()
{
	// -------------------------------------------------
	// Etap (2) przeslanie danych wierzcholk�w do OpenGL
	// -------------------------------------------------
	glGenVertexArrays( 1, &idVAO );
	glBindVertexArray( idVAO );

	// Bufor na wspolrzedne wierzcholkow
	glGenBuffers( 1, &idVBO_coord );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Mesh_Vertices ), Mesh_Vertices, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	glBindVertexArray( 0 );


	// ---------------------------------------
	// Etap (3) stworzenie potoku graficznego
	// ---------------------------------------
	idProgram = glCreateProgram();
	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));
	LinkAndValidateProgram( idProgram );



	// -----------------------------------------
	// Etap (4) ustawienie maszyny stanow OpenGL
	// -----------------------------------------

	// ustawienie koloru czyszczenia ramki koloru
	glClearColor( 0.9f, 0.9f, 0.9f, 0.9f );

}

// ---------------------------------------
// Funkcja wywolywana podczas zmiany rozdzielczosci ekranu
void Reshape( int resW, int resH )
{
	glViewport( 0, 0, resW, resH );
	width = resW;
	height = resH;
}



// ---------------------------------------------------
// Funkcja wywolywana podczas wcisniecia klawisza ASCII
void Keyboard( unsigned char key, int x, int y )
{
    switch(key)
    {
		case 27:	// ESC key
			// opuszczamy glowna petle
			glutLeaveMainLoop();
			break;

		case ' ':	// Space key
			printf("SPACE!\n");
			// ponowne renderowanie

			break;

    }

    glutPostRedisplay();
}


// ---------------------------------------------------
int main( int argc, char *argv[] )
{
	// -----------------------------------------------
	// Etap (1) utworzynie kontektu i okna aplikacji
	// -----------------------------------------------

	// GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitWindowSize( width, height );
	glutCreateWindow( "Szablon programu w OpenGL" );
	glutDisplayFunc( DisplayScene );
	glutReshapeFunc( Reshape );
	glutKeyboardFunc( Keyboard );


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

	// Inicjalizacja
	Initialize();



	glutMainLoop();


	// Cleaning
	glDeleteProgram( idProgram );
	glDeleteVertexArrays( 1, &idVBO_coord );
	glDeleteVertexArrays( 1, &idVAO );

	return 0;
}
