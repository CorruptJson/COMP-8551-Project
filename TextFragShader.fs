#version 330 core

// input from the vert shader
// name must match the output from
// the vert shader
in vec2 TexCoord;

// final color to be displayed
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
   vec4 sampled = vec4(1.0,1.0,1.0, texture(text, TexCoord).r)
   FragColor = vec4(textColor, 1.0) * sampled;
}
