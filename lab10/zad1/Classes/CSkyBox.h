#pragma once

#include "global.h"
using namespace std;

class CSkyBox
{
    GLuint SkyBox_VAO;
    GLuint idCubeTexture;
public:
    void CreateSkyBox(const char *  posX,const char *  posXn,const char *  posY,const char *  posYn,const char *  posZ,const char *  posZn){
        GLfloat positions[8*3] =
        {
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f
            };

        GLuint indices[12*3] =
        {
            5, 0, 1,
            5, 4, 0,
            2, 0, 3,
            2, 1, 0,
            7, 0, 4,
            7, 3, 0,
            3, 6, 2,
            3, 7, 6,
            1, 2, 6,
            1, 6, 5,
            4, 5, 6,
            4, 6, 7
        };

        const char* files[6] =
        {
            posX,
            posXn,
            posY,
            posYn,
            posZ,
            posZn,

        };

        const GLenum targets[6] =
        {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
        };
        // Vertex arrays
        glGenVertexArrays( 1, &SkyBox_VAO );
        glBindVertexArray( SkyBox_VAO );

        // Wspolrzedne wierzchokow
        GLuint vBuffer_pos;
        glGenBuffers( 1, &vBuffer_pos );
        glBindBuffer( GL_ARRAY_BUFFER, vBuffer_pos );
        glBufferData( GL_ARRAY_BUFFER, 8*3*sizeof(GLfloat), positions, GL_STATIC_DRAW );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 0 );

        // Tablica indeksow
        GLuint vBuffer_idx;
        glGenBuffers( 1, &vBuffer_idx );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vBuffer_idx );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 12*3*sizeof( GLuint ), indices, GL_STATIC_DRAW );

        glBindVertexArray( 0 );

        // Tekstura CUBE_MAP
        glGenTextures( 1, &idCubeTexture);
        glBindTexture( GL_TEXTURE_CUBE_MAP, idCubeTexture );
        stbi_set_flip_vertically_on_load (false);

        // Utworzenie 6 tekstur dla kazdej sciany
        for (int i=0; i < 6; ++i)
        {
            int tex_width, tex_height, n;
            unsigned char *tex_data;
            string name = "Resources/Textures/" + string(files[i]);
            tex_data = stbi_load(name.c_str(), &tex_width, &tex_height, &n, 0);
            if (tex_data == NULL)
            {
                printf("Image %s can't be loaded!\n", files[i]);
                //exit(1);
            }
            // Zaladowanie danych do tekstury OpenGL
            glTexImage2D( targets[i], 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);

            // Zwolnienie pamieci pliku graficznego
            stbi_image_free(tex_data);

        }

        // Przykladowe opcje tekstury
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT );
        glGenerateMipmap( GL_TEXTURE_CUBE_MAP );
        glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
    };
    void DrawSkyBox(glm::mat4 matProj,glm::mat4 matView){
    	// Przeskalowanie boxa i przeslanie macierzy rzutowania
		glm::mat4 matPVM = matProj * matView * glm::scale(glm::mat4(1), glm::vec3(40.0, 40.0, 40.0));

		glUniformMatrix4fv( glGetUniformLocation( _currentProgramId, "matPVM" ), 1, GL_FALSE, glm::value_ptr(matPVM) );

		// Aktywacja tekstury CUBE_MAP
		glActiveTexture(GL_TEXTURE1);
		glBindTexture( GL_TEXTURE_CUBE_MAP, idCubeTexture );
		glUniform1i(glGetUniformLocation(_currentProgramId, "tex_skybox"), 1);

		// Rendering boxa
		glBindVertexArray( SkyBox_VAO );
		glDrawElements( GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, NULL );
		glBindVertexArray( SkyBox_VAO );

}
};
