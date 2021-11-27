#version 330 core

// input from the vert shader
// name must match the output from
// the vert shader
in vec2 TexCoord;

// final color to be displayed
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform float time;

void main() {
    float f_range = (sin(time) + 1.0)/2.0;
    vec4 color = texture(ourTexture, TexCoord);
    color.a *= f_range;
    FragColor = color;//texture(ourTexture, TexCoord);// * f_range;
}
