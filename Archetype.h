#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include "Types.h"

// an instance of an archetype object is simply a validated collection of component types
// it is like a blueprint for a certain type of entity
// you must give the entity coordinator an archetype when you create an entity
// so it knows which components the entity will use
class Archetype
{
private:

    // archetypetype is a simple integer used for faster comparison
    // arhcetypetype is always the same for the same combination of components
    ArchetypeType type;
    std::vector<ComponentType> components;

public:
    Archetype();
    Archetype(ArchetypeType type, std::vector<ComponentType> components);
    ArchetypeType getType() const;
    const std::vector<ComponentType>& getComponentTypeArray() const;
    bool hasComponentType(ComponentType type);
};
