#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct Position
{
    float x;
    float y;
};

struct Scale
{
    float x;
    float y;
};


using Rotation = float;

class Transform
{
public:
    Transform();
    Transform(float xPos, float yPos, float rot, float xScale, float yScale);
    Position getPosition();
    void setPosition(float x, float y);
    void translate(float x, float y);

    Scale getScale();
    void setScale(float x, float y);

    Rotation getRotation();
    void setRotation(float r);
    void rotate(float rDelta);

    glm::mat4 getModelMatrix();
private:
    Position position;
    Scale scale;
    Rotation rotation;
    glm::mat4 modelMatrix;

    // for optimizing matrix creation
    // reset after every getModelMatrix()
    bool positionChanged;
    bool scaleChanged;
    bool rotationChanged;

    // matrix related
    void generateModelMatrix();
    void translateModelMatrix();
};
