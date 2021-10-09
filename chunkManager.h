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
    std::unordered_map<Signature, std::vector<Chunk>> chunksPerSig;

public:

    ChunkManager()
    {

    }

    ChunkAddress assignNewEntity(Signature sig)
    {
        auto find = chunksPerSig.find(sig);
        //Chunk* c;
        if (find == chunksPerSig.end())
        {
            // make new chunk
        }
    };

    void releaseEntity(ChunkAddress id)
    {
        throw std::exception("function not implemented");
    };

};
