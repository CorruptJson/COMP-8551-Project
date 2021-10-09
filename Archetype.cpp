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
