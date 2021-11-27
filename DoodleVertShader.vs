#version 330 core

// vertex data. Each location is 
// defined by the vertex attrib pointers
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;

// data for the Frag Shader
// name must be the same as the input for Frag Shader
out vec2 TexCoord;

// uniforms passed in
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float time;

//creates noise through a randomizer using sin
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float snap(float x, float snap){
    return snap * round(x/snap);
}

void main()
{
   TexCoord = inTexCoord;

   float _time = snap(time, 5);
   float noise = rand(TexCoord + _time) * 0.0125 - 0.008;
   TexCoord.xy += noise;

    // Set gl_Position with transformed vertex position
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPos, 1);
}
