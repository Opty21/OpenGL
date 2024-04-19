#pragma once
#include "global.h"
#include "CTexture.h"
#include <list>
#include "CMaterial.h"
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <typeinfo>
#include "Ground.h"
using namespace std;
class CMesh
{
public:
    GLuint idVAO;		// tablic wierzcholkow
    GLuint idVBO_coord;
    GLuint instanceVBO;

    GLuint vBuffer_uv;
    GLuint vBuffer_normal;

	vector<glm::vec3> OBJ_vertices;
    vector<glm::vec2> OBJ_uvs;
    vector<glm::vec3> OBJ_normals;
    glm::mat4 matModel;
    glm::mat3 matNormal;
    glm::vec3 initPos;
    glm::vec3 initRotation;
    vector<CTexture> textureList;
    vector<glm::mat4x4> matModelArray;
    CGround ground;
    bool isGroundSet = false;
    CMaterial material;
	// Metody tworzaca VAO i VBO z pliku OBJ
	void CreateFromOBJ(const char *  filename){
        matModel = glm::mat4(1.0);
        initPos = glm::vec3(0,0,0);
        initRotation = glm::vec3(0,0,0);
        string name = "Resources/Models/" + string(filename);
        if (!loadOBJ(name.c_str(), OBJ_vertices, OBJ_uvs, OBJ_normals))
            {
                printf("Not loaded!\n");
                //exit(1);
            }

        createBuffers();

    };
    void setGround(CGround gr){
        ground = gr;
        isGroundSet = true;
    }
    void createFromMatrix(vector<vector<char>> matrix,int scale){
        matModel = glm::mat4(1.0);
        initPos = glm::vec3(0,0,0);
        initRotation = glm::vec3(0,0,0);
        int hor=0;
        int ver=0;

        //vertexy
        for (auto const &row: matrix) {
            for (char const &character: row) {
                std::cout << character << std::endl;
                if(character != '#' && character != ' '){
                    OBJ_vertices.push_back(glm::vec3(ver,0,hor));
                    OBJ_vertices.push_back(glm::vec3(ver+scale,0,hor));
                    OBJ_vertices.push_back(glm::vec3(ver+scale,0,hor+scale));
                    OBJ_vertices.push_back(glm::vec3(ver,0,hor));
                    OBJ_vertices.push_back(glm::vec3(ver,0,hor+scale));
                    OBJ_vertices.push_back(glm::vec3(ver+scale,0,hor+scale));
                }

                ver+=1 * scale;      
            }
            std::cout << "lul" << std::endl;
            hor+=1 * scale;
            ver=0;        
        }

        //Normale
        for (auto const &vert: OBJ_vertices) {
            OBJ_normals.push_back(glm::vec3(0,1,0));
        }

        //Textury
        for(int i=0;i<OBJ_vertices.size()/6;i++){
            OBJ_uvs.push_back(glm::vec2(0,0));
            OBJ_uvs.push_back(glm::vec2(1,0));
            OBJ_uvs.push_back(glm::vec2(1,1));
            OBJ_uvs.push_back(glm::vec2(0,0));
            OBJ_uvs.push_back(glm::vec2(0,1));
            OBJ_uvs.push_back(glm::vec2(1,1));
        }

        createBuffers();

        return;
    };

    void createBuffers(){
                glGenVertexArrays( 1, &idVAO );
        glBindVertexArray( idVAO );
        // Bufor na wspolrzedne wierzcholkow
        glGenBuffers( 1, &idVBO_coord );
        glBindBuffer( GL_ARRAY_BUFFER, idVBO_coord );
        glBufferData( GL_ARRAY_BUFFER, OBJ_vertices.size() * sizeof(glm::vec3), &OBJ_vertices[0], GL_STATIC_DRAW );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 0 );

        // Wspolrzedne textury UV
        glGenBuffers( 1, &vBuffer_uv );
        glBindBuffer( GL_ARRAY_BUFFER, vBuffer_uv );
        glBufferData( GL_ARRAY_BUFFER, OBJ_uvs.size() * sizeof(glm::vec2), &OBJ_uvs[0], GL_STATIC_DRAW );
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 1 );

        //normalne
        glGenBuffers( 1, &vBuffer_normal );
        glBindBuffer( GL_ARRAY_BUFFER, vBuffer_normal );
        glBufferData( GL_ARRAY_BUFFER, OBJ_normals.size() * sizeof(glm::vec3), &OBJ_normals[0], GL_STATIC_DRAW );
        glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 2 );
    }
    vector<string> splitString(const string& str){
        vector<string> tokens;
    
        stringstream ss(str);
        string token;
        while (getline(ss, token, '\n')) {
            tokens.push_back(token);
        }
        return tokens;
    }
    void instantiate(int amount,int scaleTop,float scaleDivider,glm::vec3 rotationAxis){
        glm::mat4x4 tempMatrix = glm::mat4(1.0f);
        for (int i=0; i<amount; ++i){
            float x = rand() % 100 - 50;
            float z = rand() % 100 - 50;
            float y=0;
            if(isGroundSet){
                y = ground.getAltitute(glm::vec2(x,z),y);
            }else{
                y = rand() % 100 - 50;
            }
            
            float scale = (rand() % scaleTop + 1)/scaleDivider;
            float angle = rand() % 90;

            tempMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z) );
            tempMatrix = glm::rotate(tempMatrix, angle, rotationAxis);
            tempMatrix = glm::scale(tempMatrix, glm::vec3(scale, scale, scale));
            matModelArray.push_back(tempMatrix);
        }
        setInstantiated();

    }
    
    void addToMatrixArray(glm::vec3 pos,glm::vec3 rotAxis,float rotAmount , float scale){
        glm::mat4x4 tempMatrix = glm::mat4(1.0f);
        tempMatrix = glm::translate(glm::mat4(1.0f), pos );
        tempMatrix = glm::rotate(tempMatrix, glm::radians(rotAmount), rotAxis);
        tempMatrix = glm::scale(tempMatrix, glm::vec3(scale, scale, scale));
        matModelArray.push_back(tempMatrix);

    }
    void setInstantiated(){
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, matModelArray.size() * sizeof(glm::mat4), &matModelArray[0], GL_STATIC_DRAW);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
    }

    void updateBuffer(){
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, matModelArray.size() * sizeof(glm::mat4), &matModelArray[0], GL_STATIC_DRAW);
    }
    
	void CreateFromOBJ(const char *  filename,glm::vec3 initP,glm::vec3 initR){
        CreateFromOBJ(filename);
        initPos = initP;
        initRotation = initR;
    };

	// Oczywiscie metoda generujaca obraz na ekranie
	void Draw(){
        		// Obiekt z pliku OBJ
        for(int i=0;i<textureList.size();i++){
            textureList[i].activateTexture(_currentProgramId,i);
        }
        material.activate();
        matNormal=glm::mat3(transpose(inverse(matModel)));
        glUniformMatrix3fv( glGetUniformLocation(_currentProgramId, "matNormal"), 1, GL_FALSE, &matNormal[0][0]);
        glUniformMatrix4fv( glGetUniformLocation(_currentProgramId, "matModel"), 1, GL_FALSE, &matModel[0][0]);
        glBindVertexArray( idVAO );
        if(matModelArray.size() == 0){
            glUniform1i ( glGetUniformLocation (_currentProgramId , "instantiated"), 0);
            glDrawArrays( GL_TRIANGLES, 0, OBJ_vertices.size() );
        }else{
            glUniform1i ( glGetUniformLocation (_currentProgramId , "instantiated"), 1);
            glDrawArraysInstanced( GL_TRIANGLES, 0, OBJ_vertices.size(), matModelArray.size());
        }
		
		glBindVertexArray( 0 );


        
    };
    void attachTexture(CTexture tex){
        textureList.push_back(tex);
    }
    void attachMaterial( glm::vec3 Ambient,glm::vec3 Diffuse, glm::vec3 Specular, float Shininess){
        material.Ambient = Ambient;
        material.Diffuse = Diffuse;
        material.Specular = Specular;
        material.Shininess = Shininess;
    }
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
