#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "sigArch.h"
#include "Types.h"
#include "Archetype.h"
#include "ComponentManager.h"

const int ENTITIES_PER_CHUNK = 32;

class Chunk
{
    friend class ChunkManager;

private:
    int chunkID = -1;
    Signature sig;
    Archetype arch;
    Spritesheet spritesheet;
    int currEnts = 0;
    std::unordered_map<ComponentType, Byte*> componentArrays{};
    int versions[ENTITIES_PER_CHUNK]{};
    int entToDat[ENTITIES_PER_CHUNK]{};
    int datToEnt[ENTITIES_PER_CHUNK]{};

    Chunk(Signature sig, int chunkID, Spritesheet spriteSheet);
    Chunk(Archetype archetype, int chunkID, Spritesheet spriteSheet, ComponentSizeMap& sizemap);

    void addComponentArrays(Archetype t, ComponentSizeMap& sizemap);

    template<typename T>
    void addComponentArray();

    template<typename T, typename ... args>
    void addComponentArray();

public:

    template<typename T, typename ... args>
    //static friend Chunk* createChunk(int chunkID, Archetype arch, Spritesheet spriteSheet, ComponentSizeMap& sizemap);

    Chunk() = delete;

    const char* GetSpritesheet();

    int getCurrEntCount();

    ChunkAddress assignNewEntity();

    void releaseEntity(ChunkAddress id);

    template<typename T>
    T& getComponentReference(ChunkAddress id);

    template <typename T>
    T* getComponentArray();

    Archetype getArchetype();

    ~Chunk();
};


