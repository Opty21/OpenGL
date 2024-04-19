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
#include "CMaterial.h"
#include "CLight.h"

int _currentProgramId;
bool _showLightSpheres = false;
// Macierze transformacji i rzutowania
glm::mat4 matProj;
glm::mat4 matView;
glm::mat4 matModel;

CMesh road, mon,candle,candle2,coolGuy,christmasTree,coolGuy3,plane;
CProgram litTex,unlitTex,litShaded,unlitShaded,pColor;
CLight light1;
float value = 1;
float counterVal=0;
int valueDirection = 1;
float animSpeed = 0.005f;
bool lightOn = true;
CTexture metalPlates,christmasTreeTex;
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

	matModel = glm :: rotate ( matModel, 1.5f * valueDirection , glm :: vec3 (0 ,1 ,0));
	matModel = glm::translate(matModel,glm::vec3(0,13,value));
	glm::vec3 cameraPosition = ExtractCameraPos(matView);
	if(lightOn){
		litTex.use();
		litTex.SetVP(matView,matProj);
		light1.activate();
		glUniform3fv( glGetUniformLocation( _currentProgramId, "cameraPosition" ), 1, &cameraPosition[0] );
		road.Draw();
		plane.Draw();
		christmasTree.Draw();
		litShaded.use();
		litShaded.SetVP(matView,matProj);
		light1.activate();
		glUniform3fv( glGetUniformLocation( _currentProgramId, "cameraPosition" ), 1, &cameraPosition[0] );
		mon.Draw();
		candle.Draw();
		candle2.Draw();
		coolGuy.Draw();

		pColor.use();
		pColor.SetVP(matView,matProj);
		light1.drawSphere();
	} else{
		unlitTex.use();
		unlitTex.SetVP(matView,matProj);
		glUniform3fv( glGetUniformLocation( _currentProgramId, "cameraPosition" ), 1, &cameraPosition[0] );
		road.Draw();
		plane.Draw();
		christmasTree.Draw();
		unlitShaded.use();
		unlitShaded.SetVP(matView,matProj);
		glUniform3fv( glGetUniformLocation( _currentProgramId, "cameraPosition" ), 1, &cameraPosition[0] );
		mon.Draw();
		candle.Draw();
		candle2.Draw();
		coolGuy.Draw();
	}



	glUseProgram( 0 );


	glutSwapBuffers();

}

// ---------------------------------------
void Initialize()
{
	
	metalPlates.CreateTexture("cer.png");
	christmasTreeTex.CreateTexture("christmas.png");


	//Light
	light1.set(glm::vec3(0.1 , 0.1 , 0.1),glm::vec3(1 , 1 , 1), glm::vec3(1.0 , 1.0 , 1.0),glm::vec3(1.0 , 0.0 , 0.0), glm::vec3(2.0 , 3.0 , 1.0));

	glEnable(GL_DEPTH_TEST);
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
	//Programs
	litTex.CreateProgram("vertex.glsl","fragmentLightTexture.glsl");
	unlitTex.CreateProgram("vertexUnlit.glsl","fragmentTex.glsl");
	litShaded.CreateProgram("vertex.glsl","fragmentLight.glsl");
	unlitShaded.CreateProgram("vertexUnlit.glsl","fragment3.glsl");
	pColor.CreateProgram("vertexUnlit.glsl","fragmentColor.glsl");
	//Textures
	//Objects
	road.CreateFromOBJ("scene.obj");
	//road.rotateMesh(0,1,0,2.1f);
	//road.translateMesh(0,14,5);
	road.attachTexture(metalPlates);
	road.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
	
	//MON
	mon.CreateFromOBJ("MONSTRUM.obj");
	mon.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
	mon.rotateMesh(0,1,0,0.5f);
	mon.translateMesh(0,0,30);

	//candle
	candle.CreateFromOBJ("voxelCandleStand2.obj");
	candle.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
	candle.setMeshPosition(-14,-0.3f,-8);
	candle.scaleMesh(3,3,3);
	
	//candle 2
	candle2.CreateFromOBJ("voxelCandleStand2.obj");
	candle2.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
	candle2.setMeshPosition(7,-0.3f,-14);
	candle2.scaleMesh(3,3,3);
	
	//coolGuy
	coolGuy.CreateFromOBJ("coolGuy.obj");
	coolGuy.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
	coolGuy.translateMesh(-40,20,-80);
	coolGuy.rotateMesh(0,1,0,0.5f);
	coolGuy.scaleMesh(25,25,25);

	//christmasTree
	christmasTree.CreateFromOBJ("christmas-tree.obj");
	//christmasTree.translateMesh(0,-1,35);

	christmasTree.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
	christmasTree.attachTexture(christmasTreeTex);
	//christmasTree.rotateMesh(0,1,0,glm::radians(90.0));
	christmasTree.translateMesh(0,-1,35);
	christmasTree.scaleMesh(8,8,8);

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
	light1.Position.y = sin(value)*5;

	//glClearColor( 0, 0, cos(value), 1.0f );

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
void Menu( int value )
{
    switch( value )
    {
		case 1:
			printf("Swiatla:ON\n");
			lightOn = true;
			break;

		case 2:
			printf("Swiatla:OFF\n");
			lightOn = false;
		case 3:
			printf("Kule swiatla: ON\n");
			_showLightSpheres = true;
			break;

		case 4:
			printf("Kule swiatla: OFF\n");
			_showLightSpheres = false;
			break;
		default:
			printf("Wybrano %d \n", value);
    }
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

	int podmenuA = glutCreateMenu( Menu );
	glutAddMenuEntry( "tak", 1 );
	glutAddMenuEntry( "nie", 2 );
	int podmenuB = glutCreateMenu( Menu );
	glutAddMenuEntry( "tak", 3 );
	glutAddMenuEntry( "nie", 4 );
    glutCreateMenu( Menu );
	glutAddSubMenu( "Renderowanie swiatla", podmenuA );
	glutAddSubMenu( "Kule swiatla", podmenuB );

    glutAttachMenu( GLUT_RIGHT_BUTTON );
	
	Initialize();

	glutTimerFunc (100 , Animation , 0);

	glutMainLoop();


	// Cleaning
	// glDeleteProgram( idProgram );
	// glDeleteVertexArrays( 1, &idVBO_coord );
	// glDeleteVertexArrays( 1, &idVAO );

	return 0;
}
