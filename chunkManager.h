#pragma once

#include <iostream>
#include <exception>
#include <unordered_map>
#include <vector>
#include "Types.h"
#include "SigArch.h"
#include "tempPosition.h"
#include "EntityCoordinator.h"
#include "chunk.h"




class ChunkManager
{
private:
    EntityCoordinator* entCoord;
    std::unordered_map<Signature, std::vector<Chunk>> chunksPerSig;

public:

    ChunkManager(EntityCoordinator* entCoord)
    {
        this->entCoord = entCoord;
    }

    ChunkAddress allocateNewEntity(Signature sig)
    {
        auto find = chunksPerSig.find(sig);
        //Chunk* c;
        if (find == chunksPerSig.end())
        {
            // make new chunk
        }
    };

    void deallocateEntity(ChunkAddress id)
    {
        throw std::exception("function not implemented");
    };

    TempPosition getEntityPosition(ChunkAddress id)
    {
        throw std::exception("function not implemented");
    };

    template<typename... Args>
    void getSigFromComponents(Args... args)
    {
        Signature sig;
        recursiveSetSig(sig, args...);
        std::string sigString = SignatureToString(sig);
        std::cout << "Recursive Set Sig is: " << sigString << std::endl;
    }

    template<typename T>
    void recursiveSetSig(Signature& sig, T component)
    {
        entCoord->GetComponentManager().SetSignatureBit(component, sig);
    }

    template<typename T, typename... Args>
    void recursiveSetSig(Signature& sig, T component, Args... args)
    {
        entCoord->GetComponentManager().SetSignatureBit(component, sig);
        recursiveSetSig(sig, args...);
    }

    std::string SignatureToString(Signature sig)
    {
        std::string s;
        for (int i = sig.size() - 1; i >= 0; i--)
        {
            s += sig[i] ? '1' : '0';
        }
        return s;
    }

    // test functions that list out components passed as arguments

    template<typename T>
    void identifyComponents(T component)
    {
        const char* typeName = typeid(component).name();
        ComponentType type = entCoord->GetComponentManager().GetComponentType<T>();

        std::cout << "type name: " << typeName << ", type num: " << unsigned(type) << std::endl;
    }

    template<typename T, typename... Args>
    void identifyComponents(T component, Args... args)
    {
        const char* typeName = typeid(component).name();
        ComponentType type = entCoord->GetComponentManager().GetComponentType<T>();

        std::cout << "type name: " << typeName << ", type num: " << unsigned(type) << std::endl;

        identifyComponents(args...);
    }
};
