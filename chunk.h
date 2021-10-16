#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
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
    void addComponentArray()
    {
        char const* typeName = typeid(T).name();
        componentArrays.insert(typeName, new T[ENTITIES_PER_CHUNK]);
    }

    template<typename T, typename ... args>
    void addComponentArray()
    {
        char const* typeName = typeid(T).name();
        componentArrays.insert(typeName, new T[ENTITIES_PER_CHUNK]);

        addComponentArray<args...>();
    }

public:

    //template<typename T, typename ... args>
    //static friend Chunk* createChunk(int chunkID, Archetype arch, Spritesheet spriteSheet, ComponentSizeMap& sizemap);

    Chunk() = delete;

    const char* GetSpritesheet();

    int getCurrEntCount();

    EntityID assignNewEntity();

    void releaseEntity(EntityID id);

    template<typename T>
    T& getComponentReference(EntityID id)
    {
        if (id.index >= ENTITIES_PER_CHUNK)
        {
            // throw error
            throw "getComponentReference: entity id index too large";
        }
        int datIndex = entToDat[id.index];
        if (datIndex >= ENTITIES_PER_CHUNK)
        {
            // throw error
            throw "getComponentReference: entity data index too large";
        }

        T* compArr = getComponentArray<T>();
        return compArr[datIndex];
    }

    template <typename T>
    T* getComponentArray()
    {
        ComponentType type = ComponentManager::NEW_GetComponentType<T>();
        auto find = componentArrays.find(type);
        if ( find  == componentArrays.end())
        {
            // type is not in chunk component type array map
            throw "type is not in chunk component type array map";
        }
        Byte* arr = find->second;
        //T* compArr = std::static_cast<T*>(arr);
        T* compArr = (T*)arr;
        return compArr;
    }

    Archetype getArchetype();

    ~Chunk();
};


