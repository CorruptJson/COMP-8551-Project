#pragma once
#include <bitset>

// ECS
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
using ArchetypeType = std::uint16_t;
using ComponentSize = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;
using SpriteSheet = const char*;

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


