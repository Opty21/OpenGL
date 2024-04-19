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
#include "Classes/LevelManager.h"
#include "Classes/CLight.h"
#include "Classes/CSkyBox.h"
#include <time.h>
#include "Classes/global.h"
#include "Classes/Ground.h"
#include "Classes/Camera.h"

#include "text-ft.h"
#include "Classes/Player.h"

int _currentProgramId;
bool _showLightSpheres = false;
int _tileScale =5;
// Macierze transformacji i rzutowania
glm::mat4 matProj;
glm::mat4 matView;
glm::mat4 matViewPrev;

glm::mat4 matModel;

CMesh road, mon,candle,candle2,coolGuy,christmasTree,coolGuy3,plane,circle1,circle2,level,fence, ground,VHStape,guitar,sphere,screen,screen2;
CProgram litTex,unlitTex,litShaded,unlitShaded,pColor,skyboxProgram,litTexEnv;
CSkyBox sky1,sky2;
int skyStatus = 1;
CLight light1;
const int Number_of_Birds = 1000;
unsigned int vInstances;
CTexture metalPlates,christmasTreeTex,circle1Tex,circle2Tex,groundTex,VHStapeTex,guitarTex;
CGround GMesh;
CPlayer player;
CFPSCamera cam;
int frame=0,timeD,timebase=0;
float animVal = 0;
GLuint FrameBufferID;
GLuint DepthBufferID;
GLuint TextureBuffer;

#include "ShadowPointLight.h"
CShadowPointLight myLight;

LevelManager lm;
bool _levelCleared = false;
bool interactable = false;

float pushSpeed = 0.05f;
int currentlyMoving = 99999;
glm::mat4 matViewMinimap;
bool renderParticles = true;
int w=600,h=800;
void displayFPS(),drawToScene(),drawToTexture(),drawShadowMap(),manageParticles(),manageCollisions();
// ---------------------------------------
void DisplayScene()
{
	//drawShadowMap();
	// Czyszczenie bufora koloru i glebokosci
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor(	141/255.0f, 105/255.0f, 122/255.0f,1);
	glm::vec3 cameraPosition = ExtractCameraPos(cam.matView);

	drawToTexture();
	drawToScene();

	glUseProgram( 0 );
	RenderText("level: " +to_string(lm.currentLevel), 10, 40, 0.1f, glm::vec3(255, 233, 255)/255.0f);
	RenderText("press 'R' to restart", 10, 5, 0.1f, glm::vec3(255, 233, 255)/255.0f);

	RenderText(lm.levelName, 10, 25, 0.1f, glm::vec3(255, 233, 255)/255.0f);

   	//displayFPS();
	if(interactable){
		    RenderText("PRESS 'E' TO PUSH", 90, 420, 0.3f, glm::vec3(232, 255, 255)/255.0f);

	}
	if(_levelCleared){
			RenderText(" Stage Cleared ", 80, 360, 0.4f, glm::vec3(23, 233, 95)/255.0f);

	}
	cam.Update();

	glutSwapBuffers();

}

void drawToTexture(){
	glViewport(0, 0, 1600, 900);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	matViewMinimap = glm::rotate( glm::mat4(1.0), glm::radians(90.f) , glm::vec3( 1.0f, 0.0f, 0.0f ) );	
	//matViewMinimap = glm::rotate( matViewMinimap, glm::radians(180.f) , glm::vec3( 0.0f, 1.0f, 0.0f ) );	

	matViewMinimap = glm::translate( matViewMinimap, lm.calculateCameraPositionForMap() );
	glm::vec3 cameraPosition = ExtractCameraPos(matViewMinimap);
	glm::vec3 color = glm::vec3(66, 110, 93)/255.0f;

	pColor.use();
	pColor.SetVP(matViewMinimap,cam.matProj);
	
	glUniform3fv( glGetUniformLocation( _currentProgramId, "cameraPosition" ), 1, &cameraPosition[0] );
	glUniform3fv( glGetUniformLocation( _currentProgramId, "color" ), 1, &color[0]);
	player.Draw();
	//ground.Draw();
	color = glm::vec3(255, 255, 235)/255.0f;
	glUniform3fv( glGetUniformLocation( _currentProgramId, "color" ), 1, &color[0]);
	lm.level.Draw();
	color = glm::vec3(50, 61, 79)/255.0f;
	glUniform3fv( glGetUniformLocation( _currentProgramId, "color" ), 1, &color[0]);
	
	lm.box.translateAllInstantiated(glm::vec3(0,-2,0),false);
	lm.box.Draw();
	lm.fences.Draw();
	lm.box.translateAllInstantiated(glm::vec3(0,2,0),false);

	color = glm::vec3(255, 170, 94)/255.0f;
	glUniform3fv( glGetUniformLocation( _currentProgramId, "color" ), 1, &color[0]);
	lm.goal.translateAllInstantiated(glm::vec3(0,2,0),false);
	lm.goal.Draw();
	lm.goal.translateAllInstantiated(glm::vec3(0,-2,0),false);

	skyboxProgram.use();
	//sky1.DrawSkyBox(cam.matProj,matViewMinimap);
}

void drawToScene(){
	glViewport(0, 0, w, h);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glm::vec3 cameraPosition = ExtractCameraPos(cam.matView);

	litTexEnv.use();

	cam.Update();
	light1.activate();
	//myLight.SendUniform(_currentProgramId, 4, cameraPosition);

	glUniform3fv( glGetUniformLocation( _currentProgramId, "cameraPosition" ), 1, &cameraPosition[0] );
	glUniform1i(glGetUniformLocation(_currentProgramId, "tex_skybox"), 1);
	ground.Draw();
	lm.fences.Draw();
	lm.goal.Draw();
	guitar.Draw();
	unlitTex.use();
	cam.Update();
	candle.Draw();
	glUniform3fv( glGetUniformLocation( _currentProgramId, "cameraPosition" ), 1, &cameraPosition[0] );
	lm.level.Draw();
	VHStape.Draw();
	//player.Draw();
	lm.box.Draw();
	pColor.use();
	cam.Update();
	glm::vec3 color = glm::vec3(84, 78, 104)/255.0f;
	glUniform3fv( glGetUniformLocation( _currentProgramId, "color" ), 1, &color[0]);

	screen.Draw();

	unlitTex.use();
	cam.Update();
	if(renderParticles){
		circle1.Draw();
		circle2.Draw();
	}


	glBindTexture ( GL_TEXTURE_2D , TextureBuffer );
	glUniform1i ( glGetUniformLocation ( _currentProgramId ,"tex0"), 0 );
	screen2.Draw();
	skyboxProgram.use();
	light1.drawSphere();
	switch(skyStatus){
		case 1:
			sky1.DrawSkyBox(cam.matProj,cam.matView);
			break;
		default:

			break;
    }
}

void drawShadowMap(){
	myLight.GenBegin();
	ground.Draw();
	lm.fences.Draw();
	lm.goal.Draw();
	guitar.Draw();
	lm.level.Draw();
	VHStape.Draw();
	lm.box.Draw();
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
    RenderText(txt, 25, 200, 0.5f, glm::vec3(255, 170, 94)/255.0f);

}
// ---------------------------------------

void updatePositions(){
	glm::vec3 temp = glm::vec3((lm.longestLine*_tileScale)/2.0f,0,(lm.height*_tileScale)/2.0f);
	ground.setMeshPosition(temp.x,temp.y,temp.z);
	sky1.translateVec = temp;
	guitar.translateAllInstantiated(temp,true);
	circle1.translateAllInstantiated(temp,true);
	circle2.translateAllInstantiated(temp,true);
	VHStape.translateAllInstantiated(temp,true);
	screen.matModel = lm.calculateScreenPos();
	screen2.matModel = lm.calculateScreenPos();
	//myLight.Position = temp + glm::vec3(0,8,0);
}
void Initialize()
{

	player.CreateFromOBJ("christmas-tree.obj");
	cam.Init(glm::vec3(0,5,0),player);
	ground.CreateFromOBJ("omegaGround.obj");
	ground.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
	ground.attachTexture(groundTex);
	GMesh.Init(ground.OBJ_vertices);


	// player.setGround(GMesh);

	lm.init(1,"Maps\\Magic_Sokoban_1.sok",player);

		//Textures
	metalPlates.CreateTexture("cer.png");
	//christmasTreeTex.CreateTexture("controller.png");
	circle1Tex.CreateTexture("circle4.png");
	circle2Tex.CreateTexture("circle.png");
	VHStapeTex.CreateTexture("vhs.png");
    InitText("Audiowide-Regular.ttf", 100);
	groundTex.CreateTexture("ground.png");

	//Light
	light1.set(glm::vec3(0.1 , 0.1 , 0.1),glm::vec3(1 , 1 , 1), glm::vec3(0, 187/255.0f, 204/255.0f),glm::vec3(1.0 , 0.0 , 0.0), glm::vec3(2.0 , 3.0 , 1.0));

	//myLight.Init(glm::vec3(0.0, 15.0, 0.0));

	glEnable(GL_DEPTH_TEST);
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
	//Programs
	unlitTex.CreateProgram("vertexUnlit.glsl","fragmentTex.glsl");
	pColor.CreateProgram("vertexUnlit.glsl","fragmentColor.glsl");
	skyboxProgram.CreateProgram("SkyBox-vertex.glsl","SkyBox-fragment.glsl");
	litTexEnv.CreateProgram("vertex.glsl","fragmentLightTextureEnv.glsl");
	
	//Skybox
	sky1.CreateSkyBox("sky169/sky169ft.png","sky169/sky169bk.png","sky169/sky169up.png","sky169/sky169dn.png","sky169/sky169rt.png","sky169/sky169lf.png");

	//Objects

	screen.CreateFromOBJ("screen.obj");
	screen.attachTexture(groundTex);
	screen2.CreateFromOBJ("screen2.obj");


	VHStape.CreateFromOBJ("kasetaVHS.obj");
	VHStape.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
	VHStape.attachTexture(VHStapeTex);
	VHStape.setGround(GMesh);	
	VHStape.instantiate(50,4,5,glm::vec3(1,1,1));

	guitar.CreateFromOBJ("guitar.obj");
	guitar.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
	guitar.attachTexture(metalPlates);
	guitar.setGround(GMesh);	
	guitar.instantiate(50,15,1,glm::vec3(1,1,1));


	//instantiated circle1s
	circle1.CreateFromOBJ("flower.obj");
	circle1.attachTexture(circle1Tex);
	//circle1.setGround(GMesh);
	circle1.instantiate(400,3,10,glm::vec3(1,1,1));

	circle2.CreateFromOBJ("flower.obj");
	circle2.attachTexture(circle2Tex);
	circle2.instantiate(400,3,10,glm::vec3(1,1,1));
	//circle1.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );

	updatePositions();

	
	glGenTextures(1, &TextureBuffer);
	glBindTexture(GL_TEXTURE_2D, TextureBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1600, 900, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenFramebuffers(1, &FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

	// Polaczenie tekstury do skladowej koloru bufora ramki
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureBuffer, 0);
	// 3. Stworzenie obiektu render-buffer dla testu glebokosci

	glGenRenderbuffers(1, &DepthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1600, 900);
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
	
	glutPostRedisplay();
	//myLight.UpdatePV();
	candle.translateMesh(0.03f,0,0);
	if(_restartLevel){
		_restartLevel = false;
		lm.box.currMoveValue = 0;
		lm.box.currentlyMoving = false;
		currentlyMoving = 99999;

		lm.init(lm.currentLevel,lm.filename,player);
		
	}
	
	//Player movement stuff
	player.Move(playerMovement * player.movementSpeed);
	player.Rotate(-_scene_rotate_y);
	_scene_rotate_y = 0;
	cam.xRot -= _scene_rotate_x;
	_scene_rotate_x = 0;
	cam.Update();
	//playerMovement =0;
	playerRotation = 0;

	//MANAGE COLLISIONS
	manageCollisions();

	//SNAP MOUSE TO CENTER
	if(!inMenu)glutWarpPointer(GLUT_WINDOW_WIDTH/2,GLUT_WINDOW_HEIGHT/2);

	//MANAGE PARTICLES
	if(renderParticles) manageParticles();

	
	if(currentlyMoving != 99999){
		if(lm.box.currMoveValue < _tileScale){
			lm.box.matModelArray[currentlyMoving] = glm::translate(lm.box.matModelArray[currentlyMoving],lm.box.currMoveDirection * pushSpeed);
			lm.box.currMoveValue += pushSpeed;
			lm.box.updateBuffer();
		}
		else{
			lm.box.currMoveValue = 0;
			lm.box.currentlyMoving = false;
			currentlyMoving = 99999;
			//NEXT LEVEL 
			if(_levelCleared){
				_levelCleared = false;
 				lm.init(lm.currentLevel +1,lm.filename,player);
				updatePositions();
			}
		}
	}
	// animVal += 0.002f;

	// light1.Position.z = 15 * sin(animVal);

	glutTimerFunc (10 , Animation , 0);
}

void manageCollisions(){
	interactable = false;
	player.colliding = false;
	tuple<bool,bool> temp;
	glm::vec3 nextPos = player.Position +  player.Direction * playerMovement * player.movementSpeed;
	for(int i = 0; i < lm.box.matModelArray.size();i++){
		temp = lm.box.checkForCollision(i,nextPos);
		if(get<0>(temp) == true){
			interactable = true;
			if(interacting){
				interacting = false;
				player.colliding = true;

				if(currentlyMoving == 99999) {
					glm::vec3 boxPos = glm::vec3(lm.box.matModelArray[i][3].x,lm.box.matModelArray[i][3].y,lm.box.matModelArray[i][3].z);
					glm::vec3 norm = glm::round(glm::normalize(boxPos - player.Position));
					if(abs(norm.x) - abs(norm.z) == 0  ) return;
					if(!lm.makeMove(boxPos,norm)) return;
					if(!_levelCleared) _levelCleared = lm.checkForWin();
					currentlyMoving = i;
					lm.box.currentlyMoving = true;
					lm.box.currMoveDirection = norm;
				}
			}
		}
		if(get<1>(temp) == true){
			player.colliding = true;
		}
	}
}

void manageParticles(){
		for(int i =0 ; i< circle1.matModelArray.size();i++){
		circle1.matModelArray[i] = glm::translate(circle1.matModelArray[i],glm::vec3(0,-0.01f,0));
		if(glm::vec3(circle1.matModelArray[i][3]).y < -10){
			circle1.matModelArray[i] = glm::translate(circle1.matModelArray[i],glm::vec3(0,30,0));
		} 
		circle1.matModelArray[i] = glm::rotate(circle1.matModelArray[i], glm::radians(0.3f) , glm :: vec3 (0 ,1 ,0));
	}
	circle1.updateBuffer();

	for(int i =0 ; i< circle2.matModelArray.size();i++){
		circle2.matModelArray[i] = glm::translate(circle2.matModelArray[i],glm::vec3(0,-0.01f,0));
		if(glm::vec3(circle2.matModelArray[i][3]).y < -10){
			circle2.matModelArray[i] = glm::translate(circle2.matModelArray[i],glm::vec3(0,30,0));
		} 
		circle2.matModelArray[i] = glm::rotate(circle2.matModelArray[i], glm::radians(0.3f) , glm :: vec3 (0 ,1 ,0));
	}
	circle2.updateBuffer();
}

void Menu( int value )
{
	inMenu = false;
	glutSetCursor(GLUT_CURSOR_NONE);

    switch( value )
    {
		case 1:
			glutReshapeWindow(500,500);	
			break;

		case 2:
			glutFullScreen();
	
			break;
		case 3:
			renderParticles = false;			
			break;

		case 4:
			renderParticles = true;			
			break;
		case 5:
			skyStatus = 0;
			glActiveTexture(GL_TEXTURE1);
			glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
			break;
		case 6:
			skyStatus = 1;
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
	glutPassiveMotionFunc(MouseMotion);
	glutMouseWheelFunc( MouseWheel );
    glutKeyboardFunc( keyboard_down );
    glutKeyboardUpFunc( keyboard_up );
	glutSpecialFunc( SpecialKeys );
	glutSetCursor(GLUT_CURSOR_NONE);
	glutFullScreen();


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
	glutAddMenuEntry( "Nie", 1 );
	glutAddMenuEntry( "Tak", 2 );
	int podmenuB = glutCreateMenu( Menu );
	glutAddMenuEntry( "brak", 3 );
	glutAddMenuEntry( "Spawn", 4 );
	int podmenuC = glutCreateMenu( Menu );
	glutAddMenuEntry( "brak", 5 );
	glutAddMenuEntry( "Skybox 1", 6 );

    glutCreateMenu( Menu );
	glutAddSubMenu( "Fullscreen", podmenuA );
	glutAddSubMenu( "Particles", podmenuB );
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
