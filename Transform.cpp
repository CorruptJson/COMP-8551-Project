#define _USE_MATH_DEFINES
#include <math.h>
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
    // Convert from radians to degrees then return
    return (rotation * (180.0f/M_PI));
}

void Transform::setRotation(float r) {
    // Convert the degree value to radians so openGL can use it
    r = r * (M_PI / 180.0f);
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

    // proper math orders is scale first, then rotate, then translate.
    // However, matrix multiplication order is apply right to left, so we are
    // (codewise) translating, then rotating, then scaling.
    //model = glm::translate(model, glm::vec3(position.x, position.y, 0));

    // has to specify a rotation axis => need a review
    // do I need a quaternion? do I need to shift to accomodate
    // for whether x and y is top left or center?

    //if you want to rotate an object in-place about a local axis, 
    //you have to translate the object to the origin, perform the rotation, 
    //and then translate the object back to its original location.
    //model = glm::rotate(model, rotation, rotAxis);

    // Make sure to offset according to scale
    //model = glm::translate(model, glm::vec3(scale.x, 0));

    // translate rotated matrix back to transform's position after rotation
    model = glm::translate(model, glm::vec3(position.x, position.y, 0));

    // rotate about z-axis at the origin to avoid orbiting then translate back
    // specify the axis to be z since we're in 2-d
    // Positive angle in degrees rotates counter clockwise
    model = glm::rotate(model, rotation, glm::vec3(0, 0, 1));

    // apply a scale if there is any
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1));

    modelMatrix = model;
}

