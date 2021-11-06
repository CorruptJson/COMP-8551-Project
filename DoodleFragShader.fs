#version 330 core

// input from the vert shader
// name must match the output from
// the vert shader
in vec2 TexCoord;

// final color to be displayed
out vec4 FragColor;

uniform sampler2D ourTexture;

void main() {
   FragColor = texture(ourTexture, TexCoord);
}
