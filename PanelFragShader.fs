#version 330 core

// input from the vert shader
// name must match the output from
// the vert shader
in vec2 TexCoord;

// final color to be displayed
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 panelColor;
void main() {
   FragColor = vec4(panelColor, 1.0) + texture(ourTexture, TexCoord);//texture(ourTexture, TexCoord);
}
