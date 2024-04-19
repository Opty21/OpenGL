#pragma once

using namespace std;
class CMesh
{
public:
    GLuint idVAO;		// tablic wierzcholkow
    GLuint idVBO_coord;
	std::vector<glm::vec3> OBJ_vertices;
    std::vector<glm::vec2> OBJ_uvs;
    std::vector<glm::vec3> OBJ_normals;
    glm::mat4 matModel;
    
    glm::vec3 initPos;
    glm::vec3 initRotation;

	// Metody tworzaca VAO i VBO z pliku OBJ
	void CreateFromOBJ(const char *  filename){
        matModel = glm::mat4(1.0);
        initPos = glm::vec3(0,0,0);
        initRotation = glm::vec3(0,0,0);

        if (!loadOBJ(filename, OBJ_vertices, OBJ_uvs, OBJ_normals))
            {
                printf("Not loaded!\n");
                exit(1);
            }

        glGenVertexArrays( 1, &idVAO );
        glBindVertexArray( idVAO );
        // Bufor na wspolrzedne wierzcholkow
        glGenBuffers( 1, &idVBO_coord );
        glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
        glBufferData( GL_ARRAY_BUFFER, OBJ_vertices.size() * sizeof(glm::vec3), &OBJ_vertices[0], GL_STATIC_DRAW );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 0 );
        glBindVertexArray( 0 );
    };

	void CreateFromOBJ(const char *  filename,glm::vec3 initP,glm::vec3 initR){
        CreateFromOBJ(filename);
        initPos = initP;
        initRotation = initR;
    };

	// Oczywiscie metoda generujaca obraz na ekranie
	void Draw(int idProgram){
        		// Obiekt z pliku OBJ
        glUniformMatrix4fv( glGetUniformLocation(idProgram, "matModel"), 1, GL_FALSE, &matModel[0][0]);
        glBindVertexArray( idVAO );
		glDrawArrays( GL_TRIANGLES, 0, OBJ_vertices.size() );
		glBindVertexArray( 0 );
    };

    void translateMesh(float x, float y, float z){
	    matModel = glm::translate(matModel,glm::vec3(x,y,z));
    }
    void setMeshPosition(float x, float y, float z){
	    matModel = glm::translate(glm::mat4(1.0),glm::vec3(x,y,z));
    }
    void rotateMesh(float x, float y, float z,float amount){
	    matModel = glm :: rotate ( matModel, amount, glm :: vec3 (x ,y ,z));
    }
    void setMeshRotation(float x, float y, float z,float amount){
	    matModel = glm :: rotate ( glm::mat4(1.0), amount, glm :: vec3 (x ,y ,z));
    }
    void scaleMesh(float x, float y, float z){
        matModel = glm::scale(matModel, glm::vec3(x,y,z));
    }
    void setMeshScale(float x, float y, float z){
        matModel = glm::scale(glm::mat4(1.0), glm::vec3(x,y,z));
    }
	// Moze cos jeszcze ?
	
};