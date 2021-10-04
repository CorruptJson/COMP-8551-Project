#include <glm/glm.hpp>
#pragma once

using namespace glm;

class Camera
{
public:
    Camera(float posX, float posY, float rotZ, float zoomFactor);
    Camera();
    void moveCamera(float x, float y);
    vec2 position;
    //void rotateCamera(float x, float y);
    //void zoom(float factor);
private:
    //float rotation;
    //float zoomFactor;
};

