#include "global.h"
using namespace std;

class CProgram
{
    
public:
    GLuint idProgram;

	void CreateProgram(const char * vertShader,const char * fragShader){
	    idProgram = glCreateProgram();
        string vS = "Shaders/Vertex/" + string(vertShader);
        string vF = "Shaders/Fragment/" + string(fragShader);

        glAttachShader( idProgram, LoadShader(GL_VERTEX_SHADER, vS.c_str()));
        glAttachShader( idProgram, LoadShader(GL_FRAGMENT_SHADER,vF.c_str()));
        LinkAndValidateProgram( idProgram );
    };

	// Metoda wysylajaca zmienne uniform (model, view, projection)
	void SetVP(glm::mat4 matView, glm::mat4 matProj){
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matProj"), 1, GL_FALSE, &matProj[0][0]);
		glUniformMatrix4fv( glGetUniformLocation(idProgram, "matView"), 1, GL_FALSE, &matView[0][0]);
    };
	
    void use(){
        glUseProgram( idProgram);
        _currentProgramId = idProgram;
    }

    void setUniformInt(const char * uniName,int value){
        glUniform1i(glGetUniformLocation(idProgram, uniName), value);
    }
};
