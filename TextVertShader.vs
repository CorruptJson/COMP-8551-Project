#version 330 core

// vertex data. Each location is 
// defined by the vertex attrib pointers
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexCoord;

// data for the Frag Shader
// name must be the same as the input for Frag Shader
out vec2 TexCoord;

// uniforms passed in
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
   TexCoord = inTexCoord;
    // Set gl_Position with transformed vertex position
   //gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPos, 0.0, 1.0);
   gl_Position = projectionMatrix * vec4(inPos, 0.0, 1.0);
}