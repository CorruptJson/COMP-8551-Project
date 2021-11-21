#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
    Camera(float posX, float posY);
    Camera();
    void moveCamera(float x, float y);
    void setViewSize(int viewWidth, int viewHeight);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
private:
    bool changed;
    glm::vec2 position;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
};

