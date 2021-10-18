#pragma once

//#include "ComponentArray.h"
#include "Types.h"
//#include "componentInfo.h"
#include <any>
#include <memory>
#include <unordered_map>



class ComponentManager
{
    friend class EntityCoordinator;

private:
    ComponentSizeMap mComponentSizes;
    static ComponentType typeCounter;
    static ComponentManager* main;

public:

    ComponentManager();

    static ComponentSize GetComponentSize(ComponentType t);

    template<typename T>
    static ComponentSize GetComponentSize()
    {
        return GetGetComponentSize(GetComponentType<T>());
    }

    template<typename T>
    static ComponentType GetComponentType()
    {
        static const ComponentType typeID = typeCounter++;
        return typeID;
    }

    template<typename T>
    void RegisterComponent()
    {
        ComponentType ct = GetComponentType<T>();

        assert(mComponentSizes.find(ct) == mComponentSizes.end() && "Registering component type more than once.");
        mComponentSizes.emplace(ct, sizeof(T));
    }  

    ComponentSize getComponentSize(ComponentType t);
};
