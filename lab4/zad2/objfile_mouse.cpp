#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_stuff.h"
#include "obj_loader.h"
#include "CMesh.h"
#include "CProgram.h"

// Macierze transformacji i rzutowania
glm::mat4 matProj;
glm::mat4 matView;
glm::mat4 matModel;


// Identyfikatory obiektow
GLuint idProgram;	// programu
GLuint idVAO;		// tablic wierzcholkow
GLuint idVBO_coord;	// bufora na wspolrzedne
GLuint idVBO_color; // bufora na kolory


// -----------------------------------
// NOWE: wektory na dane z pliku OBJ
// -----------------------------------
std::vector<glm::vec3> OBJ_vertices;
std::vector<glm::vec2> OBJ_uvs;
std::vector<glm::vec3> OBJ_normals;


CMesh torus, box, cone,cone2;
CProgram p1;
float animValue = 0;
float animSpeed = 0.01f;
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
	p1.use();
	p1.SetVP(matView,matProj);
	torus.Draw(p1.idProgram);
	cone.Draw(p1.idProgram);
	cone2.Draw(p1.idProgram);
	box.Draw(p1.idProgram);
	// Wylaczanie
	glUseProgram( 0 );


	glutSwapBuffers();

}

void Animation ( int keyframe )
{
	glutPostRedisplay ();
	torus.setMeshRotation(0,0,1,animValue);
	torus.scaleMesh(0.6,5,0.6);
	
	cone.setMeshRotation(0,0,1,animValue);
	cone.translateMesh(0,5,0);

	cone2.setMeshRotation(0,0,1,animValue);
	cone2.translateMesh(0,-5,0);
	cone2.scaleMesh(-1,-1,-1);
	
	box.setMeshScale(0.5,0.5,3);
	box.translateMesh(0,0,-1);
	animValue += animSpeed;
	glutTimerFunc (10 , Animation , 0);
}

// ---------------------------------------
void Initialize()
{

	glEnable(GL_DEPTH_TEST);
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );

	// -----------------------------------
	// NOWE: wczytanie pliku obj


	// Potok
	p1.CreateProgram("vertex.glsl","fragment.glsl");
	
	cone.CreateFromOBJ("cone.obj");
	cone2.CreateFromOBJ("cone.obj");
	box.CreateFromOBJ("cube.obj");
	torus.CreateFromOBJ("torus.obj");
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

	glutTimerFunc (100 , Animation , 0);

	glutMainLoop();


	// Cleaning
	glDeleteProgram( idProgram );
	glDeleteVertexArrays( 1, &idVBO_coord );
	glDeleteVertexArrays( 1, &idVAO );

	return 0;
}
