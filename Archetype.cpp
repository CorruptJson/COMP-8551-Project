#include <string>
#include <iostream>
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

const std::vector<ComponentType> Archetype::getComponentTypes() const
{
    return components;
}

inline std::ostream& operator<<(std::ostream& str, Archetype const& arch)
{
    const ArchetypeType type = arch.getType();
    str << "archetype type: " << unsigned(type) << ", components: ";
    std::vector<ComponentType> types = arch.getComponentTypes();
    for (int i = 0; i < types.size(); i++)
    {
        str << hex_last_2(types[i])
    }
    return str;
}
