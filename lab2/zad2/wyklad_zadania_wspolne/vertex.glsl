#version 330 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec3 inColors;

// Przeslac kolor do fragment shadera


out float myColor;

void main()
{

    // zmienna typu vec4
    vec4 finalPosition = inPosition;

    myColor = inColors.x;

    // finalna pozycja wierzcholka
    gl_Position = finalPosition;

}
