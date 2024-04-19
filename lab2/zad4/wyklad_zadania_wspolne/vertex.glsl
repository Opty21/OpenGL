#version 330 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec3 inColors;

// Przeslac kolor do fragment shadera


out vec3 myColor;
uniform int Mode;

void main()
{
    switch (Mode)
      {
         case 0:
            gl_Position = vec4(-inPosition[1], -inPosition[0], inPosition[2], inPosition[3]);
            break;
         case 1:
            gl_Position = vec4(inPosition[1], inPosition[0], inPosition[2], inPosition[3]);
            break;
         case 2:
            gl_Position = vec4(inPosition[1], inPosition[0], inPosition[2], inPosition[3]);
            break; 
         case 3:
            gl_Position = vec4(inPosition[0], inPosition[1], inPosition[2], inPosition[3]);

            break;   
         default:
            gl_Position = vec4(-inPosition[1], -inPosition[0], inPosition[2], inPosition[3]);

      }


}
