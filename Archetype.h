#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
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
    const std::vector<ComponentType> getComponentTypesArrayCopy() const;
};

inline std::ostream& operator<<(std::ostream& str, Archetype const& arch)
{
    const ArchetypeType type = arch.getType();
    str << "archetype type: " << unsigned(type) << ", components: ";
    std::vector<ComponentType> types = arch.getComponentTypesArrayCopy();
    for (int i = 0; i < types.size(); i++)
    {
        str << std::hex << (int)types[i] << ' ';
    }
    return str;
}

