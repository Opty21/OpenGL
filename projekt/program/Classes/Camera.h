// -----------------------------------------------
// Przykladowy szablon budowy klasy CCamera oraz
// pochodnych do obslugi kamery
// UWAGA: nie wystarczy wkleic do swojego projektu
// nalezy uzupelnic pewne rzeczy (m.in. zaznaczone
// jako TO_DO
// -----------------------------------------------

#ifndef __CAMERA_H
#define __CAMERA_H
#include "Player.h"

#define PI	3.1415926535


// -----------------------------------------------
class CCamera
{
	public:
	// Macierze rzutowania i widoku
	glm::mat4 matProj;
	glm::mat4 matView;

	// Skladowe kontrolujace matView
	glm::vec3 Position;        // polozenie kamery
	glm::vec3 Angles;          // pitch, yaw, roll
	glm::vec3 Up;              // domyslnie wektor (0,1,0)
	glm::vec3 Direction;       // wektor kierunku obliczany z Angles

	// Skladowe kontrolujace matProj
	float Width, Height;       // proporcje bryly obcinania
	float NearPlane, FarPlane; // plaszczyzny tnace
	float Fov;	               // kat widzenia kamery

	float xRot = 0;

	// ---------------------------------------------
	CCamera()
	{

		// Domyslne wartosci skladowych
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Position = glm::vec3(0.0f, 6.0f, 15.f);
		Angles = glm::vec3(-0.5f, -1.57f, 0.0f);
		Fov = 60.0f;

		NearPlane = 0.1f;
		FarPlane = 150.0f;

		UpdateDirectionCross();
	}

	// ---------------------------------------------
	// wirtualna metoda aktualizujaca dane kamery
	// przydatna w klasach pochodnych
	virtual void Update()
	{

	}

	// ---------------------------------------------
	void UpdateMatView()
	{
		matView = glm::lookAt(Position, Position+Direction, Up);
	}

	// ---------------------------------------------
	void SetPerspective(float width, float height, float fov)
	{
		Width = width;
		Height = height;
		Fov = fov;
		matProj = glm::perspectiveFov(glm::radians(Fov), Width, Height, NearPlane, FarPlane);
	}

	// ---------------------------------------------
	void SetOrtho(float width, float height, float fov)
	{
		Width = width;
		Height = height;
		Fov = fov;
		matProj = glm::perspectiveFov(glm::radians(Fov), width, height, NearPlane, FarPlane);
	}

	// ---------------------------
	void UpdateDirectionCross()
	{
		Direction.x = cos(Angles.y) * cos(Angles.x);
		Direction.y = sin(Angles.x);
		Direction.z = -sin(Angles.y) * cos(Angles.x);
	}

	// ---------------------------------------------
	// przesylanie obu macierzy do programu
	void SendPV(GLuint program, const char *proj, const char *view)
	{
		glUniformMatrix4fv( glGetUniformLocation( program, proj ), 1, GL_FALSE, glm::value_ptr(matProj) );
		glUniformMatrix4fv( glGetUniformLocation( program, view ), 1, GL_FALSE, glm::value_ptr(matView) );
	}

	// ---------------------------------------------
	// przesylanie iloczynu macierzy do programu
	void SendPV(GLuint program, const char *projview)
	{
		glm::mat4 matProjView = matProj * matView;
		glUniformMatrix4fv( glGetUniformLocation( program, projview ), 1, GL_FALSE, glm::value_ptr(matProjView) );
	}

	// ---------------------------------------------
	void AddFov(GLfloat _fov)
	{
		Fov += _fov;
		SetPerspective(Width, Height, Fov);
	}

};


// ----------------------------------------------------------
class CFreeCamera : public CCamera
{

public:


	void Init()
	{
		// TO_DO: uzupelnij wedle uznania
	}

	void Move(float val)
	{
		Position += Direction*val;
	}


	// -----------------------------------------------
	void Rotate(float pitch, float yaw)
	{
		Angles.x += pitch;
		Angles.y += yaw;

		if (Angles.x > PI/2.0)  Angles.x =  PI/2.0;
		if (Angles.x < -PI/2.0) Angles.x = -PI/2.0;
		if (Angles.y > 2.0*PI)  Angles.y -= 2.0*PI;
		if (Angles.y < -2.0*PI) Angles.y += 2.0*PI;

		UpdateDirectionCross();
	}

};

// ----------------------------------------------------------
class CFPSCamera : public CCamera
{

public:

	// For FPS/TPS camera
	glm::vec3 ShiftUp;

	CPlayer *Player = NULL;
	CFPSCamera(){};


	// ---------------------------------------------
	void Init(glm::vec3 shUp,CPlayer& pl)
	{
		ShiftUp = shUp;
		Player = &pl;
	}
	

	// ---------------------------------------------
	// aktualizacja danych kamery na podstawie
	// danych postaci
	virtual void Update()
	{
		xRot = glm::clamp(xRot, -1.0f, 1.0f);
		Angles = glm::vec3(xRot, Player->Angle, 0.0);
		Position = Player->Position + ShiftUp;

		CCamera::SetPerspective(2000,1400,80);
		CCamera::UpdateDirectionCross();
		CCamera::UpdateMatView();
		CCamera::SendPV(_currentProgramId,"matProj","matView");
	}

};

// -------------------------------------------------------
class CTPSCamera : public CFPSCamera
{

public:

	// dodatkowe przesuniecie kamery trzecioosobowej
	glm::vec3 ShiftBack;


	// ---------------------------------------------
	void Init()
	{
		// TO_DO: uzupelnij wedle uznania
	}

	// ---------------------------------------------
	void Update()
	{

		// TO_DO: uzupelnij wedle uznania
	}

};

#endif // __CAMERA_H
