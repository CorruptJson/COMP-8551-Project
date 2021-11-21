#include "Camera.h"

Camera::Camera(float posX, float posY) {
    position = glm::vec2(posX, posY);
    projectionMatrix = glm::mat4(1);
    viewMatrix = glm::mat4(1);
}

Camera::Camera() : Camera(0, 0) { }

void Camera::setViewSize(int viewWidth, int viewHeight) {
    float CENTER_X_COORD = 0;
    float CENTER_Y_COORD = 0;
    float LEFT_X_COORD = CENTER_X_COORD - viewWidth / 2;
    float RIGHT_X_COORD = CENTER_X_COORD + viewWidth / 2;
    float BOTTOM_Y_COORD = CENTER_Y_COORD - viewHeight / 2;
    float TOP_Y_COORD = CENTER_Y_COORD + viewHeight / 2;
    float EYE_NEAR = 0.f;
    float EYE_FAR = -1.f;
    projectionMatrix = glm::ortho(LEFT_X_COORD, RIGHT_X_COORD, BOTTOM_Y_COORD, TOP_Y_COORD, EYE_NEAR, EYE_FAR);
}
    
void Camera::moveCamera(float x, float y) {
    position.x += x;
    position.y += y;
    changed = true;
}

// note that when the camera moves, we move the whole world
// in the opposite direction.
glm::mat4 Camera::getViewMatrix() {
    if (changed) {
        glm::mat4 viewMat = glm::mat4(1.0f);
        viewMatrix = glm::translate(viewMat, glm::vec3(-position.x, -position.y, 0));
        changed = false;
    }
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() {
    return projectionMatrix;
}
