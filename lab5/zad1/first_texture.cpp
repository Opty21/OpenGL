#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ------------------------------------------------------
// Pliki naglowkowe i implementacja funkcji do wczytania
// obrazow graficznych
// ------------------------------------------------------
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



#include "shader_stuff.h"


// Macierze transformacji i rzutowania
glm::mat4 matProj;
glm::mat4 matView;
glm::mat4 matModel;


// Identyfikatory obiektow
GLuint idProgram;
GLuint idVAO;
GLuint idVBO_coord;
GLuint idVBO_color;
GLuint idVBO_uv;
GLuint idTexture;
GLuint idTexture2;
GLuint idTexture3;

// ---------------------------------------
// Dane trojkata
// ---------------------------------------
GLfloat vertices_coord[] =
{
    -1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
};


GLfloat vertices_color[] =
{
	1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};


GLfloat vertices_tex[] =
{
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.5f, 1.0f,
};





// ---------------------------------------
void DisplayScene()
{
	// Czyszczenie bufora koloru i glebokosci
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Obliczanie macierz widoku (kontrolowanie polozeniem kamery)
	matView = glm::mat4x4( 1.0 );
	matView = glm::translate( matView, glm::vec3( _scene_translate_x, _scene_translate_y, _scene_translate_z) );
	matView = glm::rotate( matView, _scene_rotate_x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	matView = glm::rotate( matView, _scene_rotate_y, glm::vec3( 0.0f, 1.0f, 0.0f ) );


	// Wlaczenie programu
	glUseProgram( idProgram );


		// Przekazanie macierzy
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matProj"), 1, GL_FALSE, &matProj[0][0]);
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matView"), 1, GL_FALSE, &matView[0][0]);


		// Obiekt z pliku OBJ
		matModel = glm::mat4(1.0);
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matModel"), 1, GL_FALSE, &matModel[0][0]);


		// -----------------------------------------------------------------------------
		// Krok 5. Aktywacja jednostki teksturuj�cej, przypisanie odpowiedniego obiektu
		//         tekstury oraz po��czenie jej z uchwytem tekstury w fragment shaderze.
		// -----------------------------------------------------------------------------
		glActiveTexture ( GL_TEXTURE0 );
		glBindTexture ( GL_TEXTURE_2D , idTexture );
		glUniform1i ( glGetUniformLocation ( idProgram , "tex"), 0 );
		glActiveTexture ( GL_TEXTURE1 );
		glBindTexture ( GL_TEXTURE_2D , idTexture2 );
		glUniform1i ( glGetUniformLocation ( idProgram , "tex2"), 1 );
		glActiveTexture ( GL_TEXTURE2 );
		glBindTexture ( GL_TEXTURE_2D , idTexture3 );
		glUniform1i ( glGetUniformLocation ( idProgram , "tex3"), 2 );
		// rendering
		glBindVertexArray( idVAO );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		glBindVertexArray( 0 );


	// Wylaczanie
	glUseProgram( 0 );


	glutSwapBuffers();

}

// ---------------------------------------
void Initialize()
{

	// Ustawienia maszyny OpenGL
	glEnable(GL_DEPTH_TEST);
	glClearColor( 0.7f, 0.7f, 0.7f, 1.0f );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	// Potok
	idProgram = glCreateProgram();
	glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
	glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));
	LinkAndValidateProgram( idProgram );


	// VAO
	glGenVertexArrays( 1, &idVAO );
	glBindVertexArray( idVAO );
	// Bufor na wspolrzedne wierzcholkow
	glGenBuffers( 1, &idVBO_coord );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices_coord), &vertices_coord[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	// Bufor na kolory
	glGenBuffers( 1, &idVBO_color );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_color );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices_color), &vertices_color[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );

	// --------------------------------------------------------------------------
	// Krok 1. Utworzenie bufora w ramach VAO obiektu na wsp�rz�dne UV tekstur.
	// --------------------------------------------------------------------------
	// Bufor na UV
	glGenBuffers( 1, &idVBO_uv );
	glBindBuffer( GL_ARRAY_BUFFER, idVBO_uv );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices_tex), &vertices_tex[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 2 );


	glBindVertexArray( 0 );


	// -----------------------------------------------------
	// Krok 3. Wczytanie pliku graficznego obrazu.
	// -----------------------------------------------------

	int tex_width, tex_height, tex_n;
	unsigned char * tex_data;

	stbi_set_flip_vertically_on_load (true);
	tex_data = stbi_load ("brick.png", &tex_width , &tex_height , &tex_n, 0);

	int tex_width2, tex_height2, tex_n2;

	unsigned char * tex_data2;
	tex_data2 = stbi_load ("dogg.png", &tex_width2 , &tex_height2 , &tex_n2, 0);

	int tex_width3, tex_height3, tex_n3;
	
	unsigned char * tex_data3;
	tex_data3 = stbi_load ("snieg.png", &tex_width3 , &tex_height3 , &tex_n3, 0);

	if ( tex_data == NULL || tex_data2 == NULL || tex_data3 == NULL)  {
		printf (" Image can �t be loaded !\n");
		exit (1);
	}

	// -----------------------------------------------------
	// Krok 4. Utworzenie obiektu tekstury w OpenGL.
	// -----------------------------------------------------

	glGenTextures (1, &idTexture );
	glBindTexture ( GL_TEXTURE_2D , idTexture );

	glTexImage2D ( GL_TEXTURE_2D , 0, GL_RGBA , tex_width , tex_height , 0, GL_RGBA , GL_UNSIGNED_BYTE , tex_data );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glGenerateMipmap ( GL_TEXTURE_2D );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_BASE_LEVEL , 0);

	// -----------------------------------------------------
	// Krok 4. Utworzenie obiektu tekstury w OpenGL. v2
	// -----------------------------------------------------

	glGenTextures (1, &idTexture2 );
	glBindTexture ( GL_TEXTURE_2D , idTexture2 );

	glTexImage2D ( GL_TEXTURE_2D , 0, GL_RGBA , tex_width2 , tex_height2 , 0, GL_RGBA , GL_UNSIGNED_BYTE , tex_data2 );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glGenerateMipmap ( GL_TEXTURE_2D );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_BASE_LEVEL , 0);

	glGenTextures (1, &idTexture3 );
	glBindTexture ( GL_TEXTURE_2D , idTexture3 );

	glTexImage2D ( GL_TEXTURE_2D , 0, GL_RGBA , tex_width3 , tex_height3 , 0, GL_RGBA , GL_UNSIGNED_BYTE , tex_data3 );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glGenerateMipmap ( GL_TEXTURE_2D );
	glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_BASE_LEVEL , 0);

}

// ---------------------------------------
void Reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
	matProj = glm::perspective(glm::radians(80.0f), width/(float)height, 0.1f, 50.0f);
}



// ---------------------------------------------------
int main( int argc, char *argv[] )
{
	// GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow( "Programownie grafiki w OpenGL" );


	glutDisplayFunc( DisplayScene );
	glutReshapeFunc( Reshape );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutMouseWheelFunc( MouseWheel );
	glutKeyboardFunc( Keyboard );
	glutSpecialFunc( SpecialKeys );


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


	glutMainLoop();


	// Cleaning
	glDeleteProgram( idProgram );
	glDeleteVertexArrays( 1, &idVBO_coord );
	glDeleteVertexArrays( 1, &idVAO );

	return 0;
}
