#pragma once
#include <bitset>
#include <unordered_map>
#include <string>
#include <iostream>

// ECS
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
using ComponentSize = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using ComponentSizeMap = std::unordered_map<ComponentType, ComponentSize>;

using ArchetypeType = std::uint16_t;

using Signature = std::bitset<MAX_COMPONENTS>;
using Spritesheet = const char*;

using Byte = unsigned char;

//struct ComponentInfoStruct
//{
//    ComponentType type;
//    Signature sig;
//    ComponentSize size;
//};

struct ChunkAddress
{
    int chunkID;
    int index;
    int version;
};

inline std::ostream& operator<<(std::ostream& str, ChunkAddress const& ca)
{
    str << "chunk: " << ca.chunkID << ", index: " << ca.index << ", ver: " << ca.version;
    return str;
}


