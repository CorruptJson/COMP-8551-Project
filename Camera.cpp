#include "Camera.h"

Camera::Camera(float posX, float posY, float rotZ, float zoomFactor) {
    position = vec2(posX, posY);
}

Camera::Camera() {
    position = vec2(0.0f, 0.0f);
}

void Camera::moveCamera(float x, float y) {
    position.x += x;
    position.y += y;
}

mat4 Camera::getViewMatrix() {
    mat4 modelMatrix = mat4(1.0f);
    return translate(modelMatrix, vec3(position.x, position.y, 0));
}

//void Camera::rotateCamera(float x, float y) {
//
//}
//
//void Camera::zoom(float factor) {
//
//}
