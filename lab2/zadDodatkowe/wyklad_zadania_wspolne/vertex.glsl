#version 330 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec3 inColors;

// Przeslac kolor do fragment shadera


out vec3 myColor;

void main()
{

    // zmienna typu vec4
    vec4 finalPosition = inPosition;

    myColor = inColors;

    // finalna pozycja wierzcholka
    gl_Position = finalPosition;

}
