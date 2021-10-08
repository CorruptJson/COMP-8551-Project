#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main()
{
   ourColor = inColor;
   TexCoord = inTexCoord;
    // Set gl_Position with transformed vertex position
   gl_Position = viewMatrix * modelMatrix * vec4(inPos, 1);
}
