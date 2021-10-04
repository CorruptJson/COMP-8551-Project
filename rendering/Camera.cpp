#include "Camera.h"

Camera::Camera(float posX, float posY, float rotZ, float zoomFactor) {
    position = vec2(posX, posY);
}

Camera::Camera() {
    position = vec2(0.0f, 0.0f);
}

void Camera::moveCamera(float x, float y) {

}

//void Camera::rotateCamera(float x, float y) {
//
//}
//
//void Camera::zoom(float factor) {
//
//}
