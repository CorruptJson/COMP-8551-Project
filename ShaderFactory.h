#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "file_manager.h"
#include <vector>

enum class ShaderName {
    DEFAULT,
    TEXT,
    DOODLE,
    UI
};

struct Shader {
    GLuint program;
    std::vector<GLuint> uniformLocations;
};

class ShaderFactory {
public:
    void createAllShaderPrograms();

    /// <summary>
    /// Use the default shader. This includes calling the glUseProgram and loading all the uniforms.
    /// </summary>
    /// <param name="modelMat"></param>
    /// <param name="viewMat"></param>
    /// <param name="projectMat"></param>
    /// <param name="color"></param>
    void useDefaultShader(glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projectMat, glm::vec3 color, bool colorOnly);

    /// <summary>
    /// Use the text shader. This includes calling the glUseProgram and loading all the uniforms.
    /// </summary>
    /// <param name="modelMat"></param>
    /// <param name="viewMat"></param>
    /// <param name="projectMat"></param>
    /// <param name="color"></param>
    void useTextShader(glm::mat4 modelMat, glm::mat4 projectMat, glm::vec3 color);

    /// <summary>
    /// Use the text shader. This includes calling the glUseProgram and loading all the uniforms.
    /// </summary>
    /// <param name="modelMat"></param>
    /// <param name="viewMat"></param>
    /// <param name="projectMat"></param>
    /// <param name="color"></param>
    void useDoodleShader(glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projectMat, float time);
private:
    std::map<ShaderName, Shader> shaders;

    void createShaderProgram(ShaderName name, std::string vertPath, std::string fragPath, std::string uniforms[], int amount);
};
