#version 330 core

// input from the vert shader
// name must match the output from
// the vert shader
in vec2 TexCoord;

// final color to be displayed
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 color;
uniform bool colorOnly;

void main() {
    if (colorOnly) {
        FragColor = vec4(color, 1.0);
        return;
    }
    vec4 texColor = texture(ourTexture,TexCoord);

    if(texColor.a < 0.1)
        discard;

    FragColor = texColor * vec4(color, 1.0);
}
