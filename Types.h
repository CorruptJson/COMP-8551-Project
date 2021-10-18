#pragma once
#include <bitset>
#include <unordered_map>
#include <string>
#include <iostream>

// ECS
//using Entity = std::uint32_t;
//const Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
using ComponentSize = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using ComponentSizeMap = std::unordered_map<ComponentType, ComponentSize>;

using ArchetypeType = std::uint16_t;

using Spritesheet = const char*;

using Byte = unsigned char;


struct EntityID
{
    int chunkID;
    int index;
    int version;
};

// enables us to directly print entityID as a string into cout
inline std::ostream& operator<<(std::ostream& str, EntityID const& ca)
{
    str << "chunk: " << ca.chunkID << ", index: " << ca.index << ", ver: " << ca.version;
    return str;
}


