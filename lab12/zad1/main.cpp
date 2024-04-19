#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader_stuff.h"
#include "obj_loader.h"
#include "Classes/CMesh.h"
#include "Classes/CProgram.h"
#include "Classes/CTexture.h"
#include "Classes/CMaterial.h"
#include "Classes/CLight.h"
#include "Classes/CSkyBox.h"
#include <time.h>
#include "Classes/global.h"
#include "Classes/Player.h"
#include "Classes/Ground.h"
#include "text-ft.h"

int _currentProgramId;
bool _showLightSpheres = false;
// Macierze transformacji i rzutowania
glm::mat4 matProj;
glm::mat4 matView;
glm::mat4 matViewPrev;

glm::mat4 matModel;

CMesh road, mon,candle,candle2,coolGuy,christmasTree,coolGuy3,plane,santa,grMesh;
CProgram litTex,unlitTex,litShaded,unlitShaded,pColor,skyboxProgram,litTexEnv;
CSkyBox sky1,sky2;
int skyStatus = 0;
CLight light1;
float value = 1;
float counterVal=0;
int valueDirection = 1;
float animSpeed = 0.005f;
bool lightOn = true;
const int Number_of_Birds = 1000;
unsigned int vInstances;
CTexture metalPlates,christmasTreeTex,santaFace,groundTex;

CGround ground;
CPlayer player;

int frame=0,timeD,timebase=0;

GLuint FrameBufferID;
GLuint DepthBufferID;
GLuint TextureBuffer;

#include "ShadowPointLight.h"
CShadowPointLight myLight;

int w=600,h=800;
void displayFPS(),drawToScene(),drawToTexture(),drawShadowMap();
// ---------------------------------------
void DisplayScene()
{
	drawShadowMap();
	// Czyszczenie bufora koloru i glebokosci
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Obliczanie macierz widoku (kontrolowanie polozeniem kamery)
	matView = glm::mat4x4( 1.0 );
	matView = glm::translate( matView, glm::vec3( _scene_translate_x, _scene_translate_y, _scene_translate_z) );
	matView = glm::rotate( matView, _scene_rotate_x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	matView = glm::rotate( matView, _scene_rotate_y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	matViewPrev = matView;
	

	glm::vec3 cameraPosition = ExtractCameraPos(matView);

	matView = glm::rotate( glm::mat4(1.0), glm::radians(90.f) , glm::vec3( 1.0f, 0.0f, 0.0f ) );	
	matView = glm::translate( matView, glm::vec3( cameraPosition.x, -50, -cameraPosition.z) );
	drawToTexture();
	matView = matViewPrev;
	
	
	//  plane.setMeshPosition( cameraPosition.x, cameraPosition.y, cameraPosition.z);
	// //plane.setMeshRotation(1,0,0,glm::radians(90.f));
	// //plane.rotateMesh(0,1,0,_scene_rotate_x);

	// plane.rotateMesh(0,1,0,-_scene_rotate_y);
	// plane.rotateMesh(1,0,0,-_scene_rotate_x);

	//  plane.translateMesh(0,0,-0.2f);
	//  plane.scaleMesh(0.05f,0.05f,0.05f);
	// // plane.rotateMesh(1,0,0,glm::radians(90.f));
	//  plane.translateMesh(2.f,2.f,0);

	drawToScene();

	glUseProgram( 0 );


   	displayFPS();

	glutSwapBuffers();

}

void drawToTexture(){
	glViewport(0, 0, 300, 300);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glm::vec3 cameraPosition = ExtractCameraPos(matView);

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

void drawToScene(){
	glViewport(0, 0, w, h);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glm::vec3 cameraPosition = ExtractCameraPos(matView);

	if(lightOn){
		litTexEnv.use();
		litTexEnv.SetVP(matView,matProj);
		light1.activate();
		glUniform3fv( glGetUniformLocation( _currentProgramId, "cameraPosition" ), 1, &cameraPosition[0] );
		glUniform1i(glGetUniformLocation(_currentProgramId, "tex_skybox"), 1);
				// -------------------------------------------------------
		// NOWE: Wyslij wszystko co potrzebuje ShadowPoint
		// -------------------------------------------------------
		myLight.SendUniform(_currentProgramId, 4, cameraPosition);
		player.Draw();
		grMesh.Draw();
		//road.Draw();
		christmasTree.Draw();
		litShaded.use();
		litShaded.SetVP(matView,matProj);
		myLight.SendUniform(_currentProgramId, 4, cameraPosition);

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
		christmasTree.Draw();
		unlitShaded.use();
		unlitShaded.SetVP(matView,matProj);
		glUniform3fv( glGetUniformLocation( _currentProgramId, "cameraPosition" ), 1, &cameraPosition[0] );
		mon.Draw();
		candle.Draw();
		candle2.Draw();
		coolGuy.Draw();
	}

	unlitTex.use();
	unlitTex.SetVP(matView,matProj);
	santa.Draw();
	glBindTexture ( GL_TEXTURE_2D , TextureBuffer );
	glUniform1i ( glGetUniformLocation ( _currentProgramId ,"tex0"), 0 );
	//plane.Draw();
	skyboxProgram.use();
	
	switch(skyStatus){
		case 1:
			sky1.DrawSkyBox(matProj,matView);
			break;
		case 2:
			sky2.DrawSkyBox(matProj,matView);
			break;
		default:

			break;
    }
}

void drawShadowMap(){
	myLight.GenBegin();
	player.Draw();
	grMesh.Draw();
	//road.Draw();	
	christmasTree.Draw();
	mon.Draw();
	candle.Draw();

	candle2.Draw();
	coolGuy.Draw();
	myLight.GenEnd();

}

float fps;
void displayFPS(){
	frame++;
	timeD=glutGet(GLUT_ELAPSED_TIME);
	char txt[255];
	if (timeD - timebase > 1000) {
		fps = frame*1000.0/(timeD-timebase);
	 	timebase = timeD;
		frame = 0;
	}
	sprintf(txt, "FPS:%4.0f", fps);
    RenderText(txt, 25, 25, 0.5f, glm::vec3(255, 170, 94)/255.0f);

}
// ---------------------------------------
void Initialize()
{
	
    InitText("arial.ttf", 36);
	groundTex.CreateTexture("ground.png");

	grMesh.CreateFromOBJ("scene-levels.obj");
	grMesh.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
	grMesh.attachTexture(groundTex);
	//ground.setMeshPosition(0,-10.0f,0);
	//ground.scaleMesh(0.5f,0.5f,0.5f);
	ground.Init(grMesh.OBJ_vertices);
	
	player.CreateFromOBJ("coolGuy.obj");
	player.setGround(ground);

	//Light
	light1.set(glm::vec3(0.1 , 0.1 , 0.1),glm::vec3(1 , 1 , 1), glm::vec3(1.0 , 1.0 , 1.0),glm::vec3(1.0 , 0.0 , 0.0), glm::vec3(2.0 , 3.0 , 1.0));

	myLight.Init(glm::vec3(0.0, 2.0, 0.0));

	glEnable(GL_DEPTH_TEST);
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
	//Programs
	litTex.CreateProgram("vertex.glsl","fragmentLightTexture.glsl");
	unlitTex.CreateProgram("vertexUnlit.glsl","fragmentTex.glsl");
	litShaded.CreateProgram("vertex.glsl","fragmentLight.glsl");
	unlitShaded.CreateProgram("vertexUnlit.glsl","fragment3.glsl");
	pColor.CreateProgram("vertexUnlit.glsl","fragmentColor.glsl");
	skyboxProgram.CreateProgram("SkyBox-vertex.glsl","SkyBox-fragment.glsl");
	litTexEnv.CreateProgram("ShadowPointLight_Main.vert","ShadowPointLight_Main.frag");
	// sky1.CreateSkyBox("skybox/posx.jpg","skybox/negx.jpg","skybox/posy.jpg","skybox/negy.jpg","skybox/posz.jpg","skybox/negz.jpg");
	// sky2.CreateSkyBox("skybox4/posx.jpg","skybox4/negx.jpg","skybox4/posy.jpg","skybox4/negy.jpg","skybox4/posz.jpg","skybox4/negz.jpg");

	//Textures
	metalPlates.CreateTexture("cer.png");
	christmasTreeTex.CreateTexture("christmas.png");
	santaFace.CreateTexture("santaFace.png");
	//Objects

	plane.CreateFromOBJ("plane.obj");
	plane.translateMesh(0,3,0);
	plane.rotateMesh(1,0,0, glm::radians(-90.f));
	plane.scaleMesh(3,3,3);
	//instantiated santas
	// santa.CreateFromOBJ("flower.obj");
	// santa.attachTexture(santaFace);
	// santa.instantiate(400);
	//santa.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );



	//road
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
	//candle2.instantiate(5);
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
	christmasTree.translateMesh(0,-1,-15);
	christmasTree.scaleMesh(8,8,8);


	//minimap stuff
	glGenTextures(1, &TextureBuffer);
	glBindTexture(GL_TEXTURE_2D, TextureBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 300, 300, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenFramebuffers(1, &FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

	// Polaczenie tekstury do skladowej koloru bufora ramki
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureBuffer, 0);
	// 3. Stworzenie obiektu render-buffer dla testu glebokosci

	glGenRenderbuffers(1, &DepthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 300, 300);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Polaczenie bufora glebokosci z aktualnym obiektem bufora ramki
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBufferID);

	// --------------------------------------------------
	// 4. Sprawdzenie czy pomyslnie zostal utworzony obiekt bufora ramki
	// i powrot do domyslnego bufora ramki

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error: Framebuffer is not complete!\n");
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// ---------------------------------------
void Reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
	w = width;
	h = height;
	matProj = glm::perspective(glm::radians(80.0f), width/(float)height, 0.1f, 200.0f);
}


void Animation ( int keyframe )
{
	
	glutPostRedisplay ();
	player.Move(playerMovement * player.movementSpeed);
	player.Rotate(playerRotation * player.rotationSpeed);
	playerMovement =0;
	playerRotation = 0;
	myLight.UpdatePV();
	// for(int i =0 ; i< santa.matModelArray.size();i++){
	// 	santa.matModelArray[i] = glm::translate(santa.matModelArray[i],glm::vec3(0,-0.01f,0));
	// 	if(glm::vec3(santa.matModelArray[i][3]).y < -3){
	// 		santa.matModelArray[i] = glm::translate(santa.matModelArray[i],glm::vec3(0,30,0));
	// 	} 
	// 	santa.matModelArray[i] = glm::rotate(santa.matModelArray[i], glm::radians(0.3f) , glm :: vec3 (0 ,1 ,0));
	// 	santa.updateBuffer();
	// }
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
	glutTimerFunc (1 , Animation , 0);
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
			break;
		case 3:
			printf("Kule swiatla: ON\n");
			_showLightSpheres = true;
			break;

		case 4:
			printf("Kule swiatla: OFF\n");
			_showLightSpheres = false;
			break;
		case 5:
			skyStatus = 0;
			glActiveTexture(GL_TEXTURE1);
			glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
			break;
		case 6:
			skyStatus = 1;
			break;
		case 7:
			skyStatus = 2;
			break;
		default:
			printf("Wybrano %d \n", value);
			break;
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
	int podmenuC = glutCreateMenu( Menu );
	glutAddMenuEntry( "brak", 5 );
	glutAddMenuEntry( "Skybox 1", 6 );
	glutAddMenuEntry( "Skybox 2", 7 );

    glutCreateMenu( Menu );
	glutAddSubMenu( "Renderowanie swiatla", podmenuA );
	glutAddSubMenu( "Kule swiatla", podmenuB );
	glutAddSubMenu( "Skybox", podmenuC );

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
