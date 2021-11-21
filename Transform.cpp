#include "Transform.h"


// public 
Transform::Transform() : Transform(0, 0, 0, 1, 1) { }
Transform::Transform(float xPos=0, float yPos=0, float rot=0, float xScale=1, float yScale=1) {
    position = {
        xPos,
        yPos
    };

    rotation = rot;

    scale = {
        xScale,
        yScale
    };

    interpolX = nullptr;
    interpolY = nullptr;
    generateModelMatrix();
}

Position Transform::getPosition() const {
    return position;
}

// most commonly used function => will optimized
// by translating the modelMatrix only
void Transform::setPosition(float x, float y) {
    position = {
        x, y
    };
    changed = true;
}

void Transform::translate(float x, float y) {
    position.x += x;
    position.y += y;
    changed = true;
}

void Transform::setPhysicsBody(b2Body* newBody) {
    body = newBody;
}

b2Body* Transform::getPhysicsBody() {
    return body;
}

Scale Transform::getScale() const {
    return scale;
}

void Transform::setScale(float x, float y) {
    scale = {
        x, y
    };
    changed = true;
}

Rotation Transform::getRotation() {
    return rotation;
}

void Transform::setRotation(float r) {
    rotation = r;
    changed = true;
}

void Transform::rotate(float rDelta) {
    rotation += rDelta;
    changed = true;
}
// create a new model matrix only when we are needed.
// contains optimization so we only need to do matrix
// math when necessary
glm::mat4 Transform::getModelMatrix() {
    if (changed) {
        generateModelMatrix();
        // reset for next call
        changed = false;
    }

    return modelMatrix;
}

// private
// Generate the model matrix based on the current
// transforms at the time of call.
// Doesn't do rotation at the current moment.
void Transform::generateModelMatrix() {
    glm::mat4 model = glm::mat4(1);

    float positionX = interpolX != nullptr ? interpolX->interpolate(position.x) : position.x;
    float positionY = interpolY != nullptr ? interpolY->interpolate(position.y) : position.y;

    // proper math orders is scale first, then rotate, then translate.
    // However, matrix multiplication order is apply right to left, so we are
    // (codewise) translating, then rotating, then scaling.
    model = glm::translate(model, glm::vec3(positionX, positionY, 0));

    // has to specify a rotation axis => need a review
    // do I need a quaternion? do I need to shift to accomodate
    // for whether x and y is top left or center?
    //model = glm::rotate(model, rotation, rotAxis);

    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1));

    modelMatrix = model;
}

void Transform::setInterpolatorX(Interpolator* interpol) {
    interpolX = interpol;
    changed = true;
}

void Transform::setInterpolatorY(Interpolator* interpol) {
    interpolY = interpol;
    changed = true;
}

