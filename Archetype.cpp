#include "Archetype.h"

Archetype::Archetype()
{
    type = UINT16_MAX;    
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

const std::vector<ComponentType>& Archetype::getComponentTypeArray() const
{
    return components;
}

bool Archetype::hasComponentType(ComponentType type)
{
    auto find = std::find(components.begin(), components.end(), type);
    return (find != components.end());
}

// a function that enables Archetypes to be directly printed as a string into cout
inline std::ostream& operator<<(std::ostream& str, Archetype const& arch)
{
    const ArchetypeType type = arch.getType();
    str << "archetype type: " << unsigned(type) << ", components: ";
    std::vector<ComponentType> types = arch.getComponentTypeArray();
    for (int i = 0; i < types.size(); i++)
    {
        str << std::hex << (int)types[i] << ' ';
    }
    return str;
}
