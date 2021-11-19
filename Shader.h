#pragma once
#include <glad/glad.h>
#include <map>

// the shaders uniforms we are using
// uniforms are extra data that we pass in
// manually to the shaders. They stay 
// the same in both vertex and fragment shaders.
enum UNIFORMS {
    MODEL_MATRIX_LOCATION,
    VIEW_MATRIX_LOCATION,
    PROJECTION_MATRIX_LOCATION,
    TIME,
    NUM_OF_UNIFORMS
};

enum class ShaderName {
    DEFAULT,
    TEXT,
    DOODLE
};

struct Shader {
    GLuint Program;
    GLuint Uniforms[NUM_OF_UNIFORMS];
};
