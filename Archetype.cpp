#include "Archetype.h"

Archetype::Archetype()
{
    type = 255;    
}

Archetype::Archetype(ArchetypeType type, std::vector<ComponentType> components)
{
    this->type = type;
    this->components = components;
}

ArchetypeType Archetype::getType()
{
    return type;
}

std::vector<ComponentType> Archetype::getComponentTypes()
{
    return components;
}
