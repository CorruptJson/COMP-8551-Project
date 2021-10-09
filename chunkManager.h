#pragma once

#include <iostream>
#include <exception>
#include <unordered_map>
#include <vector>
#include "Types.h"
#include "chunk.h"

class ChunkManager
{
private:
    
    std::unordered_map<Signature, Chunk*> chunksPerSig;
    std::unordered_map<const char*, Chunk*> chunksPerSpritesheet;
    std::vector<Chunk*> allChunks;
    int currChunks = 0;

    //template<typename T, typename... Args>
    //Chunk* createAndInitChunk(Signature sig,const char* spriteSheet)
    //{
    //    Chunk* c = Chunk::createChunk<T,Args...>(currChunks, sig, spriteSheet);
    //    allChunks.push_back(c);

    //    chunksPerSig.insert(spriteSheet,c);
    //    chunksPerSpritesheet.insert(spriteSheet, c);
    //    
    //    return c;
    //}

public:

    ChunkAddress assignNewEntity(Archetype a, SpriteSheet s)
    {

    }

    //template<typename T, typename... Args>
    //ChunkAddress assignNewEntity(Signature sig, const char* spriteSheet)
    //{
    //    // are there any chunks with this signature
    //    auto find = chunksPerSig.find(sig);
    //    if (find == chunksPerSig.end())
    //    {
    //        // no, so make new chunk
    //        Chunk* c = createAndInitChunk<T, Args...>(sig, spriteSheet);
    //        ChunkAddress ca;
    //        return ca;
    //    }

    //    // are there any non-full chunks with this signature and this spritesheet?
    //    size_t bucketIndex = chunksPerSig.bucket(sig);
    //    auto begin = chunksPerSig.begin(bucketIndex);
    //    auto end = chunksPerSig.end(bucketIndex);

    //    for (auto iterator = begin;iterator != end;iterator++)
    //    {
    //        Chunk* other = iterator->second;
    //        if (other->SpriteSheet() == spriteSheet && other->getCurrEnts() < ENTITIES_PER_CHUNK)
    //        {
    //            // yes, we found a valid chunk
    //            // assign and update
    //            ChunkAddress ca;
    //            return ca;
    //        }
    //    }

    //    // no non-full chunk with matching sig and spritesheet found
    //    Chunk* c = createAndInitChunk<T, Args...>(sig, spriteSheet);
    //    ChunkAddress ca;
    //    return ca;
    //    
    //};

    void releaseEntity(ChunkAddress id)
    {
        allChunks[id.chunkID]->releaseEntity(id);
    };

    ~ChunkManager()
    {
        for (int i = 0; i < allChunks.size(); i++)
        {
            delete allChunks[i];
        }
    }
};
