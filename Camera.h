#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
    Camera(float posX, float posY);
    Camera();
    void moveCamera(float x, float y);
    void setViewSize(int viewWidth_, int viewHeight_);
    int getViewWidth();
    int getViewHeight();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
private:
    bool changed;
    int viewWidth, viewHeight;
    glm::vec2 position;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
};

