#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera
{
public:
    Camera(float posX, float posY, float rotZ, float zoomFactor);
    Camera();
    void moveCamera(float x, float y);
    mat4 getViewMatrix();
    vec2 position;
    //void rotateCamera(float x, float y);
    //void zoom(float factor);
private:
    //float rotation;
    //float zoomFactor;
};

