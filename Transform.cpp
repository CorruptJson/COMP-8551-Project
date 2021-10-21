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

    scaleChanged = true;
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
    positionChanged = true;
}

void Transform::translate(float x, float y) {
    position.x += x;
    position.y += y;
    positionChanged = true;
}

void Transform::setPhysicsBody(b2Body* newBody) {
    body = newBody;
}

b2Body* Transform::getPhysicsBody() {
    return body;
}

void Transform::update() {
    if (body && body->GetType() != b2_staticBody) {
        b2Vec2 position = body->GetPosition();
        //body->SetTransform(b2Vec2(position.x, position.y), 0);
        //setPosition(position.x, position.y);

        //printf("In transform X-Pos: %0.2f Y-Pos %0.2f\n", position.x, position.y);

    }
}

Scale Transform::getScale() const {
    return scale;
}

void Transform::setScale(float x, float y) {
    scale = {
        x, y
    };
    scaleChanged = true;
}

Rotation Transform::getRotation() {
    return rotation;
}

void Transform::setRotation(float r) {
    rotation = r;
    rotationChanged = true;
}

void Transform::rotate(float rDelta) {
    rotation += rDelta;
    rotationChanged = true;
}


// create a new model matrix only when we are needed.
// contains optimization so we only need to do matrix
// math when necessary
glm::mat4 Transform::getModelMatrix() {
    // if rotated or scaled, generate the whole matrix
    // regardless whether position changed
    if (rotationChanged || scaleChanged) {
        generateModelMatrix();
    }
    // if only position changed (the above failed)
    // then just translate => save on calculation
    else if (positionChanged) {
        translateModelMatrix();
    }
    // no change? no matrix calc

    // reset for next call
    positionChanged = false;
    rotationChanged = false;
    scaleChanged = false;

    return modelMatrix;
}

// private
// Generate the model matrix based on the current
// transforms at the time of call. This will
// check everything (position, rotation, scale).
// Use `translateModelMatrix` if you only want to
// translate it.
void Transform::generateModelMatrix() {
    glm::mat4 model = glm::mat4(1);

    // proper math orders is scale first, then rotate, then translate.
    // However, matrix multiplication order is apply right to left, so we are
    // (codewise) translating, then rotating, then scaling.
    model = glm::translate(model, glm::vec3(position.x, position.y, 0));

    // has to specify a rotation axis => need a review
    // do I need a quaternion? do I need to shift to accomodate
    // for whether x and y is top left or center?
    //model = glm::rotate(model, rotation, rotAxis);

    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1));

    modelMatrix = model;
}

void Transform::translateModelMatrix() {
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0));
    modelMatrix = model;
}


