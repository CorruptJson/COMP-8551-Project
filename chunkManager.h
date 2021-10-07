#pragma once

#include <iostream>
#include <exception>
#include "Types.h"
#include "SigArch.h"
#include "tempPosition.h"
#include "EntityCoordinator.h"

class ChunkManager
{
private:
    EntityCoordinator* entCoord;

public:

    ChunkManager(EntityCoordinator* entCoord)
    {
        this->entCoord = entCoord;
    }

    ChunkAddress allocateNewEntity(SigArch sa)
    {
        throw std::exception("function not implemented");
    };

    void deallocateEntity(ChunkAddress id)
    {
        throw std::exception("function not implemented");
    };

    TempPosition getEntityPosition(ChunkAddress id)
    {
        throw std::exception("function not implemented");
    };

    template<typename T>
    void identifyComponents(T component)
    {
        const char* typeName = typeid(component).name();
        ComponentType type = entCoord.mComponentManager;

        std::cout << "type name: " << typeName << " type num:" << type << std::endl;
    }

    template<typename T, typename... Args>
    void identifyComponents(T component, Args... args)
    {
        const char* typeName = typeid(component).name();
        ComponentType type = entCoord.mComponentManager;

        std::cout << "type name: " << typeName << " type num:" << type << std::endl;

        identifyComponents(args...);
    }
};
