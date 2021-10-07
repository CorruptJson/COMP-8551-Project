#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 TexCoord;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
void main() {
   ourColor = aColor;
   TexCoord = aTexCoord;
    // Set gl_Position with transformed vertex position
   gl_Position = viewMatrix * modelMatrix * vec4(aPos, 1);
}
