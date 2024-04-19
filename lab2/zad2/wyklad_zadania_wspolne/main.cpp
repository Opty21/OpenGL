#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "shader_stuff.h"


// ---------------------------------------
// Identyfikatory obiektow OpenGL
#define N 120
GLuint idProgram;	// programu
GLuint idProgram2;	// programu
GLuint idVAO;		// tablic wierzcholkow
GLuint idVBO_coord; // bufora na wspolrzedne
GLuint idVBO_colors; // bufora na kolory

// ---------------------------------------
// Wspolrzedne wierzchokow
GLfloat triangles[2*3*N] ={};

GLfloat colors[3*N] ={};

float Bright = 1;

// ---------------------------------------
void DisplayScene()
{
    printf("Bright = %f\n", Bright);

	// ---------------------------
	// Etap (5) rendering
	// ---------------------------
	glClear( GL_COLOR_BUFFER_BIT );


	// Wlaczenie potoku
	glUseProgram( idProgram );
    glBindVertexArray( idVAO );

		glDrawArrays( GL_TRIANGLES, 0, 3 * N / 2 );
		//glDrawArrays( GL_TRIANGLES, 3, 3 );


    glBindVertexArray( 0 );
	glUseProgram( 0 );
	// Wlaczenie potoku2
	glUseProgram( idProgram2 );
    glBindVertexArray( idVAO );

		glDrawArrays( GL_TRIANGLES, 3 * N / 2, 3 * N / 2 );
		//glDrawArrays( GL_TRIANGLES, 3, 3 );


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
	glBufferData( GL_ARRAY_BUFFER, sizeof( triangles ), triangles, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );


	// Bufor na kolory wierzcholkow
	glGenBuffers( 1, &idVBO_colors );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_colors );
	glBufferData( GL_ARRAY_BUFFER, sizeof( colors ), colors, GL_STATIC_DRAW );
	glVertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );



	glBindVertexArray( 0 );


	// ---------------------------------------
	// Etap (3) stworzenie potoku graficznego
	// ---------------------------------------
	idProgram = glCreateProgram();
	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));
	LinkAndValidateProgram( idProgram );


	// potok 2
	idProgram2 = glCreateProgram();
	glAttachShader( idProgram2, LoadShader(GL_VERTEX_SHADER, "vertex2.glsl"));
	glAttachShader( idProgram2, LoadShader(GL_FRAGMENT_SHADER, "fragment2.glsl"));
	LinkAndValidateProgram( idProgram2 );

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

			break;

        // case 'w':
        //     Bright += 0.1f;
        //     break;

        // case 's':
        //     Bright -= 0.1f;
        //     break;
    }

    glutPostRedisplay();
}

float getRandomFloat(float start, float end){
	
	return start + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(end-start)));
}
// ---------------------------------------------------
int main( int argc, char *argv[] )
{	
	srand(time(NULL));

	for(int i = 0; i < N * 2 * 3; i++){
		if(i % 6 == 0){
		triangles[i] = getRandomFloat(-1,1);
		}  else{
			triangles[i] = triangles[i - (i%6)] +  getRandomFloat(-0.2,0.3);

		}
	}

	//COLORS
	float currColor = 0;
	for(int i = 0; i< N*3; i++){
		if(i%3 == 0){
			currColor = getRandomFloat(0.3f,1);
		}
		colors[i] = currColor;
	}

	// -----------------------------------------------
	// Etap (1) utworzynie kontektu i okna aplikacji
	// -----------------------------------------------

	// GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitWindowSize( 500, 500 );
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
