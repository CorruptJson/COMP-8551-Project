#pragma once
#include <bitset>
#include <unordered_map>
#include <string>
#include <iostream>
#include "Transform.h"
#include "renderComponent.h"
#include <nlohmann/json.hpp>

// ECS

using ComponentType = std::uint8_t;
using ComponentSize = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

//using Tagset = int;

using ComponentSizeMap = std::unordered_map<ComponentType, ComponentSize>;

using ArchetypeType = std::uint16_t;

using Byte = unsigned char;

// an entity exists as a specific single index in an object pool chunk
// "version" refers to which re-used iteration of that index this specific entity existed in
// version is used to check if an entityID refers to a destroyed entity which is no longer in memory

struct EntityID
{
    int chunkID;
    int index;
    int version;

    bool isEqual(const EntityID& other) const
    {
        return other.chunkID == this->chunkID && other.index == this->index && other.version == this->version;
    }
};

inline bool operator==(const EntityID& lhs, const EntityID& rhs)
{
    return lhs.isEqual(rhs);
}

using json = nlohmann::json;

// enables us to directly print entityID as a string into cout
inline std::ostream& operator<<(std::ostream& str, EntityID const& ca)
{
    str << "chunk: " << ca.chunkID << ", index: " << ca.index << ", ver: " << ca.version;
    return str;
}

struct RenderArrays
{
    std::vector<Transform*> transforms;
    std::vector<RenderComponent*> renderComponents;
};
