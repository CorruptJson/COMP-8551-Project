#version 330 core

// vertex data. Each location is 
// defined by the vertex attrib pointers
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

// data for the Frag Shader
// name must be the same as the input for Vert Shader
out vec3 Color;
out vec2 TexCoord;

// uniforms passed in
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main()
{
   Color = inColor;
   TexCoord = inTexCoord;
    // Set gl_Position with transformed vertex position
   gl_Position = viewMatrix * modelMatrix * vec4(inPos, 1);
}
