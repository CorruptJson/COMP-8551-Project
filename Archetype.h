#pragma once
#include <vector>
#include <algorithm>
#include "Types.h"

class Archetype
{
private:

    ArchetypeType type;
    std::vector<ComponentType> components;

public:
    Archetype();
    Archetype(ArchetypeType type, std::vector<ComponentType> components);
    ArchetypeType getType() const;
    const std::vector<ComponentType> getComponentTypes() const;
};
