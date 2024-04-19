#pragma once

#include "global.h"
#include "CMesh.h"

using namespace std;

class CLight
{
    
public:
    glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
    glm::vec3 Attenuation;
    glm::vec3 Position;
    CMesh sphere;
    
    void set(glm::vec3 Amb,glm::vec3 Dif,glm::vec3 Spe,glm::vec3 Att,glm::vec3 Pos){
        Ambient = Amb;
        Diffuse = Dif;
        Specular = Spe;
        Attenuation = Att;
        Position = Pos;

        sphere.CreateFromOBJ("sphere.obj");
    }
    void activate(){
        glUniform3fv( glGetUniformLocation(_currentProgramId, "LightAmbient"), 1,glm::value_ptr(Ambient));
        glUniform3fv( glGetUniformLocation(_currentProgramId, "LightDiffuse"), 1,glm::value_ptr(Diffuse));
        glUniform3fv( glGetUniformLocation(_currentProgramId, "LightSpecular"), 1,glm::value_ptr(Specular));
        glUniform3fv( glGetUniformLocation(_currentProgramId, "LightAttenuation"), 1,glm::value_ptr(Attenuation));
        glUniform3fv( glGetUniformLocation(_currentProgramId, "LightPosition"), 1,glm::value_ptr(Position));

    };
    void drawSphere(){
        if(_showLightSpheres){
            sphere.setMeshPosition(Position.x,Position.y,Position.z);
            glUniform3fv( glGetUniformLocation(_currentProgramId, "color"), 1,glm::value_ptr(Diffuse));
            sphere.Draw();
        }
    }
};
