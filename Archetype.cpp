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

ArchetypeType Archetype::getType() const
{
    return type;
}

const std::vector<ComponentType> Archetype::getComponentTypesArrayCopy() const
{
    return components;
}

