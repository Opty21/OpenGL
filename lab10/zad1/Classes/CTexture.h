#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;
class CTexture
{
public:
    GLuint idTexture;
    int tex_width, tex_height, n;
	unsigned char *tex_data;


	void CreateTexture(const char *  filename){
        stbi_set_flip_vertically_on_load (true);

        string name = "Resources/Textures/" + string(filename);
        tex_data = stbi_load(name.c_str(), &tex_width, &tex_height, &n, 0);
        if (tex_data == NULL) {
            printf("Image %s can't be loaded!\n",filename);
            //exit(1);
        }

        // -------------------------------------------------------
        // Tworzenie obiektu tekstury
        // -------------------------------------------------------

        glGenTextures(1, &idTexture);
        glBindTexture(GL_TEXTURE_2D, idTexture);


        glTexImage2D ( GL_TEXTURE_2D , 0, GL_RGBA , tex_width , tex_height , 0, GL_RGBA , GL_UNSIGNED_BYTE , tex_data );

        glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
        glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
        glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
        glGenerateMipmap ( GL_TEXTURE_2D );
        glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_BASE_LEVEL , 0);


    };

	void activateTexture(int idProgram,int texNum){
        switch ( texNum ){
            case 0:
            glActiveTexture ( GL_TEXTURE0 );
                break;
            case 1:
            glActiveTexture ( GL_TEXTURE1 );
                break;
            case 2:
            glActiveTexture ( GL_TEXTURE2 );
                break;
            default:
            glActiveTexture ( GL_TEXTURE0 );
      }
		glBindTexture ( GL_TEXTURE_2D , idTexture );
        string uniName = "tex"+to_string(texNum);
		glUniform1i ( glGetUniformLocation ( idProgram , uniName.c_str()), texNum );
    }
	
};
