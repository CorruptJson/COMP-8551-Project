#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ostream>
#include <string>
#include "Libraries/include/box2d/box2d.h"

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
    Position getPosition() const;
    void setPosition(float x, float y);
    void translate(float x, float y);

    void setPhysicsBody(b2Body* newBody);
    b2Body* getPhysicsBody();

    Scale getScale() const;
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

    b2Body* body;

    // for optimizing matrix creation
    // reset after every getModelMatrix()
    bool changed;

    // matrix related
    void generateModelMatrix();
};

// a function that enables transform to be directly printed as a string into cout
inline std::ostream& operator<<(std::ostream& str, Transform const& t)
{
    Position p = t.getPosition();
    Scale s = t.getScale();
    str << "transform pos: " << p.x << ", " << p.y << " scale: " << s.x << ", " << s.y;
    return str;
}
