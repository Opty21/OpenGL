#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader_stuff.h"
#include "obj_loader.h"
#include "CMesh.h"
#include "CProgram.h"
#include "CTexture.h"
#include "global.h"

int _currentProgramId;
// Macierze transformacji i rzutowania
glm::mat4 matProj;
glm::mat4 matView;
glm::mat4 matModel;

CMesh road, mon,candle,candle2,coolGuy,christmasTree,coolGuy3,plane;
CProgram p1,p2,p3,pTex,pTex3;

float value = 1;
float counterVal=0;
int valueDirection = 1;
float animSpeed = 0.005f;

CTexture metalPlates,christmasTreeTex,num1,num2,num3;
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
	p1.setUniformInt("Mode",0);

	matModel = glm :: rotate ( matModel, 1.5f * valueDirection , glm :: vec3 (0 ,1 ,0));
	matModel = glm::translate(matModel,glm::vec3(0,13,value));

	p1.SetVP(matView,matProj);

	mon.Draw();
	candle.Draw();
	candle2.Draw();

	p2.use();

	p2.SetVP(matView,matProj);

	p3.use();
	p3.SetVP(matView,matProj);
	coolGuy.Draw();

	// Wylaczanie
	pTex.use();
	pTex.SetVP(matView,matProj);
	road.Draw();
	christmasTree.Draw();
	pTex3.use();
	pTex3.SetVP(matView,matProj);
	glUniform1f(glGetUniformLocation(_currentProgramId, "val"), counterVal);
	
	plane.Draw();

	glUseProgram( 0 );


	glutSwapBuffers();

}

// ---------------------------------------
void Initialize()
{
	
	metalPlates.CreateTexture("cer.png");
	christmasTreeTex.CreateTexture("christmas.png");
	num1.CreateTexture("num1.png");
	num2.CreateTexture("num2.png");
	num3.CreateTexture("num3.png");

	glEnable(GL_DEPTH_TEST);
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
	//Programs
	p1.CreateProgram("vertex.glsl","fragment.glsl");
	p2.CreateProgram("vertex.glsl","fragment2.glsl");
	p3.CreateProgram("vertex.glsl","fragment3.glsl");
	pTex.CreateProgram("vertex.glsl","fragmentTex.glsl");
	pTex3.CreateProgram("vertex.glsl","fragmentTex3.glsl");

	//Textures
	//Objects
	road.CreateFromOBJ("roadWithLanterns.obj");
	road.rotateMesh(0,1,0,2.1f);
	road.translateMesh(0,14,5);
	road.attachTexture(metalPlates);


	mon.CreateFromOBJ("MONSTRUM.obj");
	mon.rotateMesh(0,1,0,0.5f);
	mon.translateMesh(0,0,30);
	candle.CreateFromOBJ("voxelCandleStand2.obj");
	candle2.CreateFromOBJ("voxelCandleStand2.obj");
	candle.setMeshPosition(-14,-0.3f,-8);
	candle.scaleMesh(3,3,3);
	candle2.setMeshPosition(7,-0.3f,-14);
	candle2.scaleMesh(3,3,3);
	coolGuy.CreateFromOBJ("coolGuy.obj");

	coolGuy.translateMesh(-40,20,-80);
	coolGuy.rotateMesh(0,1,0,0.5f);
	coolGuy.scaleMesh(25,25,25);

	christmasTree.CreateFromOBJ("christmas-tree.obj");
	christmasTree.attachTexture(christmasTreeTex);

	christmasTree.translateMesh(0,-1,35);
	//christmasTree.rotateMesh(0,1,0,glm::radians(90.0));
	christmasTree.scaleMesh(8,8,8);

	plane.CreateFromOBJ("plane.obj");
	plane.attachTexture(num1);
	plane.attachTexture(num2);
	plane.attachTexture(num3);
	plane.translateMesh(-20,40,0);
	plane.rotateMesh(0,1,0,glm::radians(-90.0));
	plane.rotateMesh(1,0,0,glm::radians(-90.0));
	plane.scaleMesh(30,30,30);
}

// ---------------------------------------
void Reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
	matProj = glm::perspective(glm::radians(80.0f), width/(float)height, 0.1f, 200.0f);
}


void Animation ( int keyframe )
{
	glutPostRedisplay ();
	glClearColor( 0, 0, cos(value), 1.0f );

	//mon.rotateMesh(0,1,0,0.01f);
	mon.translateMesh(0,0,-0.6f);
	//mon.scaleMesh(1.02f,1.02f,1.02f);
	candle.rotateMesh(0,1,0,0.01f);
	candle2.rotateMesh(0,1,0,0.01f);
	value += animSpeed * valueDirection;
	if(value > 1){
		mon.setMeshRotation(0,1,0,0.5f);
		mon.translateMesh(0,0,35);
		//coolGuy.rotateMesh(1,0,0,-0.04);
		//coolGuy.scaleMesh(1.02,1.02,1.02);
		counterVal += animSpeed;
		if(counterVal > 2){
			counterVal = 0;
			value = 0;

		}
	}

	// if(value > 1.5 || value < 1){
	// 	valueDirection = -valueDirection;
	// }
	//mon.setMeshScale(value, value, value);
	glutTimerFunc (10 , Animation , 0);
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
	// glDeleteProgram( idProgram );
	// glDeleteVertexArrays( 1, &idVBO_coord );
	// glDeleteVertexArrays( 1, &idVAO );

	return 0;
}
