#include "ShaderFactory.h"

// info on the shaders
// note that the uniform string name and the enum matches each other
std::string defaultVertShaderPath = "DefaultVertShader.vs";
std::string defaultFragShaderPath = "DefaultFragShader.fs";
std::string defaulShaderUniforms[] = {
    "modelMatrix",
    "viewMatrix",
    "projectionMatrix",
    "color"
};
enum DEFAULT_UNIFORM {
    DEFAULT_MODEL_MAT_UNIFORM,
    DEFAULT_VIEW_MAT_UNIFORM,
    DEFAULT_PROJECT_UNIFORM,
    DEFAULT_COLOR_UNIFORM,
    DEFAULT_UNIFORM_AMOUNT
};

std::string textVertShaderPath = "TextVertShader.vs";
std::string textFragShaderPath = "TextFragShader.fs";
std::string textShaderUniforms[] = {
    "modelMatrix",
    "projectionMatrix",
    "textColor"
};
enum TEXT_UNIFORM {
    TEXT_MODEL_MAT_UNIFORM,
    TEXT_PROJECT_UNIFORM,
    TEXT_COLOR_UNIFORM,
    TEXT_UNIFORM_AMOUNT
};

std::string doodleVertShaderPath = "DoodleVertShader.vs";
std::string doodleFragShaderPath = "DoodleFragShader.fs";
std::string doodleShaderUniforms[] = {
    "modelMatrix",
    "viewMatrix",
    "projectionMatrix",
    "time"
};
enum DOODLE_UNIFORM {
    DOODLE_MODEL_MAT_UNIFORM,
    DOODLE_VIEW_MAT_UNIFORM,
    DOODLE_PROJECT_UNIFORM,
    DOODLE_TIME_UNIFORM,
    DOODLE_UNIFORM_AMOUNT
};

// functions

void ShaderFactory::createAllShaderPrograms() {
    createShaderProgram(ShaderName::DEFAULT, defaultVertShaderPath, defaultFragShaderPath, defaulShaderUniforms, DEFAULT_UNIFORM_AMOUNT);
    createShaderProgram(ShaderName::TEXT, textVertShaderPath, textFragShaderPath, textShaderUniforms, TEXT_UNIFORM_AMOUNT);
    createShaderProgram(ShaderName::DOODLE, doodleVertShaderPath, doodleFragShaderPath, doodleShaderUniforms, DOODLE_UNIFORM_AMOUNT);
}

void ShaderFactory::useDefaultShader(glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projectMat, glm::vec3 color) {
    Shader& shader = shaders[ShaderName::DEFAULT];
    glUseProgram(shader.program);

    glUniformMatrix4fv(shader.uniformLocations[DEFAULT_MODEL_MAT_UNIFORM], 1, 0, glm::value_ptr(modelMat));
    glUniformMatrix4fv(shader.uniformLocations[DEFAULT_VIEW_MAT_UNIFORM], 1, 0, glm::value_ptr(viewMat));
    glUniformMatrix4fv(shader.uniformLocations[DEFAULT_PROJECT_UNIFORM], 1, 0, glm::value_ptr(projectMat));
    glUniform3f(shader.uniformLocations[DEFAULT_COLOR_UNIFORM], color.r, color.g, color.b);
}

void ShaderFactory::useTextShader(glm::mat4 modelMat, glm::mat4 projectMat, glm::vec3 color) {
    Shader& shader = shaders[ShaderName::TEXT];
    glUseProgram(shader.program);

    glUniformMatrix4fv(shader.uniformLocations[TEXT_MODEL_MAT_UNIFORM], 1, 0, glm::value_ptr(modelMat));
    glUniformMatrix4fv(shader.uniformLocations[TEXT_PROJECT_UNIFORM], 1, 0, glm::value_ptr(projectMat));
    glUniform3f(shader.uniformLocations[TEXT_COLOR_UNIFORM], color.r, color.g, color.b);
}

void ShaderFactory::useDoodleShader(glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projectMat, float time) {
    Shader& shader = shaders[ShaderName::DOODLE];
    glUseProgram(shader.program);

    glUniformMatrix4fv(shader.uniformLocations[DOODLE_MODEL_MAT_UNIFORM], 1, 0, glm::value_ptr(modelMat));
    glUniformMatrix4fv(shader.uniformLocations[DOODLE_VIEW_MAT_UNIFORM], 1, 0, glm::value_ptr(viewMat));
    glUniformMatrix4fv(shader.uniformLocations[DOODLE_PROJECT_UNIFORM], 1, 0, glm::value_ptr(projectMat));
    glUniform1f(shader.uniformLocations[DOODLE_TIME_UNIFORM], time);
}

void ShaderFactory::createShaderProgram(ShaderName name, std::string vertPath, std::string fragPath, std::string uniforms[], int amount) {
    // init an empty shader and store the ref OpenGL returns
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertShaderSource = FileManager::readShaderFile(vertPath);
    const char* vertCStr = vertShaderSource.c_str();

    // first param is the pointer/ID that we will use the as ref
    // to the shader (the one we create above), 1 is the number of strings
    // we are storing the shader in, &vertexShaderSource is a pointer
    // to the shader code string, and NULL is the length that we will
    // read from the vertCStr => NULL means we keep reading until we see
    // a NUL EOF char.
    glShaderSource(vertexShader, 1, &vertCStr, NULL);
    glCompileShader(vertexShader);

    // check for success
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Shader Compilation Error: " << infoLog << std::endl;
    }

    // do the same thing for the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragShaderSource = FileManager::readShaderFile(fragPath);
    const char* fragCStr = fragShaderSource.c_str();

    // first param is the pointer/ID that we will use the as ref
    // to the shader, 1 is the number of strings
    // we are storing the shader in, &fragmentShaderSource is a pointer
    // to the shader code string, and NULL is unimportant
    glShaderSource(fragmentShader, 1, &fragCStr, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Shader Compilation Error: " << infoLog << std::endl;
    }

    // now that we have the shaders, we have to create and 
    // a "shader program" and attach them so it can work
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // link the program to the rendering pipeline
    glLinkProgram(shaderProgram);

    // clean up and delete the shader refs
    // we already compile and link them to the program
    // so we don't need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Shader shader;
    shader.program = shaderProgram;
    for (int i = 0; i < amount; i++) {
        shader.uniformLocations.push_back(glGetUniformLocation(shader.program, uniforms[i].c_str()));
    }
    shaders[name] = shader;
}
