#ifndef __CPLAYER
#define __CPLAYER
#include "CMesh.h"

// ----------------------------------------------------------------
// Klasa do reprezentacji obiektu, ktory porusza sie po scenie
class CPlayer : public CMesh
{

public:

	// skladowe
    glm::vec3 Position = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 Direction = glm::vec3(1.0, 0.0, 0.0);
    float Angle = 0.0;
    float movementSpeed = 0.1f;
    float rotationSpeed = 0.3f;
	// macierz modelu
    bool colliding = false;
    
    CPlayer() { }

    // Inicjalizacja obiektu
    void Init()
    {
        
    	// TO_DO: Uzupelnij wedle uznania
    }

	// Obliczenie wysokosci nad ziemia
	bool UpdateLatitute()
	{
        float temp = ground.getAltitute(glm::vec2(Position.x,Position.z),Position.y);
        if(temp == 99999){
            return false;
        } else{
            Position.y = temp;
            return true;
        }
		// TO_DO: Wykorzystaj klase CGround i zaktualizuj
		// wspolrzedna Y obiektu zgodnie z podlozem
	}


	// aktualizacja macierzy modelu
	void UpdateMatModel()
	{
		matModel = glm::translate(glm::mat4(1.0), Position);
		matModel = glm::rotate(matModel, Angle, glm::vec3(0.0, 1.0, 0.0));
	}

    // ustawienie pozycji na scenie
    void SetPosition(glm::vec3 pos)
    {
        Position = pos;
        UpdateMatModel();
    }

    // zmiana pozycji na scenie
    void Move(float val)
    {
        if(colliding)return;
        Position += Direction * val;

        // aktualizacja danych i macierzy
        bool xd = UpdateLatitute();
        if(!xd) Position -= Direction * val;
        UpdateMatModel();
    }

    void Rotate(float angle)
    {
    	Angle += angle;
		Direction.x = cos(Angle);
		Direction.z = -sin(Angle);

		// aktualizacja
		UpdateMatModel();
    }

};


#endif
