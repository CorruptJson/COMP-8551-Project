#pragma once

//#include "ComponentArray.h"
#include "Types.h"
//#include "componentInfo.h"
#include <any>
#include <memory>
#include <unordered_map>
#include <iostream>

// this class associates an ComponentType ID to specific types of components
// it also records the byte size of different component types, which is used during object pool chunk creation

// ALL COMPONENTS must be registered with the Component Manager at the beginning of engine initialization
// "getComponentType" is used by many other parts of the engine in many different areas

// the component manager can be interfaced with via the ECSCoordinator

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
        auto size = sizeof(T);
        std::uint8_t size8 = size;
        mComponentSizes.emplace(ct, size8);

        //std::cout << typeid(T).name() << " size: " << size << ", compID: " << (int)(ct) << std::endl;
    }  

    ComponentSize getComponentSize(ComponentType t);
};
