#pragma once

#include "global.h"
using namespace std;

class CMaterial
{
    
public:
    glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shininess;

    void activate(){
        glUniform3fv( glGetUniformLocation(_currentProgramId, "MatAmbient"), 1,glm::value_ptr(Ambient));
        glUniform3fv( glGetUniformLocation(_currentProgramId, "MatDiffuse"), 1,glm::value_ptr(Diffuse));
        glUniform3fv( glGetUniformLocation(_currentProgramId, "MatSpecular"), 1,glm::value_ptr(Specular));
        glUniform1f( glGetUniformLocation(_currentProgramId, "MatShininess"),Shininess);
    }
};
