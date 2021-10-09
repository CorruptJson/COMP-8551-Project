#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "sigArch.h"
#include "Types.h"
#include "Archetype.h"

const int ENTITIES_PER_CHUNK = 32;

class Chunk
{
private:
    int chunkID = -1;
    Signature sig;
    Archetype arch;
    const char* spriteSheet;
    int currEnts = 0;
    std::unordered_map<char const*, void*> componentArrays{};
    int versions[ENTITIES_PER_CHUNK]{};
    int entToDat[ENTITIES_PER_CHUNK]{};
    int datToEnt[ENTITIES_PER_CHUNK]{};

    Chunk(Signature sig, int chunkID, const char* spriteSheet, Archetype arch)
    {        
        this->sig = sig;
        this->chunkID = chunkID;
        this->spriteSheet = spriteSheet;
        this->arch = arch;
    }

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
    //static friend Chunk* createChunk(int chunkID, Signature sig, const char* spriteSheet)
    //{
    //    Chunk* chunk = new Chunk(chunkID, sig,spriteSheet);
    //    chunk->addComponentArray<T, args...>();

    //    return chunk;
    //}
    Chunk() = delete;

    const char* SpriteSheet()
    {
        return spriteSheet;
    }

    int getCurrEnts()
    {
        return currEnts;
    }

    ChunkAddress assignNewEntity()
    {
        ChunkAddress id;
        if (currEnts == ENTITIES_PER_CHUNK)
        {
            id.chunkID = -1;
            id.index = -1;
            id.version = -1;
            return id;
        }

        for (int i = 0; i < ENTITIES_PER_CHUNK; i++)
        {
            if (entToDat[i] == -1)
            {
                id.chunkID = chunkID;
                id.index = i;
                id.version = versions[i];
                entToDat[i] = currEnts;
            }
        }

        currEnts++;

        return id;
    }

    void releaseEntity(ChunkAddress id)
    {
        int dataIndex = entToDat[id.index];
        int lastIndex = currEnts - 1;
        if (dataIndex != lastIndex)
        {
            // move component data with index == lastIndex into dataIndex

            // then
            int entWithLastData = datToEnt[lastIndex];
            datToEnt[dataIndex] = entWithLastData;
            entToDat[entWithLastData] = dataIndex;
        }

        entToDat[id.index] = -1;
        versions[id.index] = versions[id.index] + 1;
        currEnts--;
    }

    template<typename T>
    T& getComponentReference(ChunkAddress id)
    {
        if (id.index >= ENTITIES_PER_CHUNK)
        {
            // throw error
        }
        int datIndex = entToDat[id.index];
        if (datIndex >= ENTITIES_PER_CHUNK)
        {
            // throw error
        }

        T* compArr = getComponentArray<T>();
        return compArr[datIndex];
    }

    template <typename T>
    T* getComponentArray()
    {
        char const* type = typeid(T).name();
        if (componentArrays.find(type) == componentArrays.end)
        {
            // type is not in chunk component type array map
        }
        void* arr = componentArrays[type];
        T* compArr = std::static_pointer_cast<T*>(arr);
        return compArr;
    }

    ~Chunk()
    {
        for (auto ca : componentArrays)
        {
            delete ca.second;
        }
    }
};
