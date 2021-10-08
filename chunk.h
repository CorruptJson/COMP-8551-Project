#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "sigArch.h"
#include "Types.h"

const int ENTITIES_PER_CHUNK = 32;

class Chunk
{
private:
    int chunkID;
    SigArch sigarch;
    int currEnts = 0;
    std::unordered_map<char const*, void*> componentArrays;
    int versions[ENTITIES_PER_CHUNK];
    int entToDat[ENTITIES_PER_CHUNK];
    int datToEnt[ENTITIES_PER_CHUNK];

public:

    //template<typename T, typename ... args>
    //Chunk(int chunkID,SigArch sigarch)
    //{
    //    this->chunkID = chunkID;
    //    this->sigarch = sigarch;

    //    const std::type_info &test = typeid(SigArch);
    //    
    //    // create array for each component type in sig arch?
    //}

    //template<typename T>
    //void addComponentArray()
    //{
    //    char const* typeName = typeid(T).name();
    //    componentArrays.insert(typeName,new T[ENTITIES_PER_CHUNK]);
    //}

    //template<typename T, typename ... args>
    //void addComponentArray()
    //{
    //    char const* typeName = typeid(T).name();
    //    componentArrays.insert(typeName, new T[ENTITIES_PER_CHUNK]);

    //    addComponentArray<...args>();
    //}

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
};
