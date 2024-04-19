#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shader_stuff.h"


// ---------------------------------------
// Identyfikatory obiektow OpenGL

GLuint idProgram;	// programu
GLuint idVAO;		// tablic wierzcholkow
GLuint idVBO_coord; // bufora na wspolrzedne
GLuint idVBO_sizes; // bufora na wspolrzedne

#define N 100
int width = 800;
int height = 600;
// ---------------------------------------
// Wspolrzedne wierzchokow
GLfloat triangles[2*N] = {};
GLfloat pointSize[N] = {};
// ---------------------------------------
void DisplayScene()
{
	// ---------------------------
	// Etap (5) rendering
	// ---------------------------
	glClear( GL_COLOR_BUFFER_BIT );


	// Wlaczenie potoku
	glUseProgram( idProgram );


		// Generowanie obiektow na ekranie
		glBindVertexArray( idVAO );
		glDrawArrays( GL_POINTS, 0, N );
		glBindVertexArray( 0 );


	// Wylaczanie
	glUseProgram( 0 );


	glutSwapBuffers();
}

// ---------------------------------------
void Initialize()
{
	// -------------------------------------------------
	// Etap (2) przeslanie danych wierzcholków do OpenGL
	// -------------------------------------------------
	glGenVertexArrays( 1, &idVAO );
	glBindVertexArray( idVAO );

	// Bufor na wspolrzedne wierzcholkow
	glGenBuffers( 1, &idVBO_coord );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
	glBufferData( GL_ARRAY_BUFFER, sizeof( triangles ), triangles, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	// Bufor na rozmiary wierzcholkow
	glGenBuffers( 1, &idVBO_sizes );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_sizes );
	glBufferData( GL_ARRAY_BUFFER, sizeof( pointSize ), pointSize, GL_STATIC_DRAW );

	glVertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );
	glEnable(GL_PROGRAM_POINT_SIZE);
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
void Reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
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
			glutPostRedisplay();
			break;
    }
}


// ---------------------------------------------------
float getRandomFloat(int start, int end){
	return start + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(end-start)));
}
int main( int argc, char *argv[] )
{
	// -----------------------------------------------
	// Etap (1) utworzynie kontektu i okna aplikacji
	// -----------------------------------------------
  	srand (time(NULL));
	
	for(int i=0; i<N * 2;i++){
		triangles[i] = getRandomFloat(-1,1);
		pointSize[i] = getRandomFloat(10,25);
		//printf("   %4.2f -=-=- %4.2f   ",triangles[i],pointSize[i]);
	}
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