#pragma once
#include<vector>
#include<unordered_map>
#include <memory>
#include "sigArch.h"
#include "entityID.h"

const int ENTS_PER_CHUNK = 32;

class Chunk
{
private:
    int chunkID;
    SigArch sigarch;
    int currEnts;
    std::unordered_map<char const*, void*> componentArrays;
    int versions[ENTS_PER_CHUNK];
    int entToDat[ENTS_PER_CHUNK];
    int datToEnt[ENTS_PER_CHUNK];
public:
    int currEnts()
    {
        return currEnts;
    }

    EntityID assignNewEntity()
    {
        EntityID id;
        if (currEnts == ENTS_PER_CHUNK)
        {
            id.chunkID = -1;
            id.index = -1;
            id.version = -1;
            return id;
        }

        for (int i = 0; i < ENTS_PER_CHUNK; i++)
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

    void releaseEntity(EntityID id)
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
    T& getComponentReference(EntityID id)
    {
        if (id.index >= ENTS_PER_CHUNK)
        {
            // throw error
        }
        int datIndex = entToDat[id.index];
        if (datIndex >= ENTS_PER_CHUNK)
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
        void* arr = componentArrays[type];
        T* compArr = std::static_pointer_cast<T*>(arr);
        return compArr;
    }
};
