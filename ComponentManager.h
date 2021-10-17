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
    //std::unordered_map<const char*, ComponentType> mComponentTypes{};
    ComponentSizeMap mComponentSizes;
    //std::unordered_map<const char*, glm::uint> mComponentInfo;
    //std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
    //ComponentType mNextComponentType{};
    static ComponentType typeCounter;
    static ComponentManager* main;

public:

    ComponentManager();

    template<typename T>
    void RegisterComponent()
    {
        //const char* typeName = typeid(T).name();
        ComponentType ct = GetComponentType<T>();

        assert(mComponentSizes.find(ct) == mComponentSizes.end() && "Registering component type more than once.");
        mComponentSizes.emplace(ct, sizeof(T));

        //mComponentTypes.insert({ typeName, mNextComponentType });
        //mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

        //Signature sig;
        //ComponentInfo info(mNextComponentType);

        //++mNextComponentType;
    }

    //template<typename T>
    //ComponentType GetComponentType()
    //{
    //    const char* typeName = typeid(T).name();

    //    assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

    //    return mComponentTypes[typeName];
    //}

    template<typename T>
    static ComponentType GetComponentType()
    {
        static const ComponentType typeID = typeCounter++;
        return typeID;
    }

    static ComponentSize GetComponentSize(ComponentType t);

    ComponentSize getComponentSize(ComponentType t);
    

    //template<typename T>
    //void AddComponent(Entity entity, T component)
    //{
    //    GetComponentArray<T>()->InsertData(entity, component);
    //}

    //template<typename T>
    //void RemoveComponent(Entity entity)
    //{
    //    GetComponentArray<T>()->RemoveData(entity);
    //}

    //template<typename T>
    //T& GetComponent(Entity entity)
    //{
    //    return GetComponentArray<T>()->GetData(entity);
    //}

    //void EntityDestroyed(Entity entity)
    //{
    //    for (auto const& pair : mComponentArrays)
    //    {
    //        auto const& component = pair.second;

    //        component->EntityDestroyed(entity);
    //    }
    //}

    //template<typename T>
    //void MakeSignature()
    //{

    //}

    //template<typename T>
    //void SetSignatureBit(Signature& sig)
    //{
    //    const char* typeName = typeid(T).name();    
    //    auto find = mComponentTypes.find(typeName);
    //    if (find == mComponentTypes.end())
    //    {
    //        // this component does not exist?
    //        std::cout << "component not registered?" << std::endl;
    //        return;
    //    }
    //    int type = find->second;
    //    sig.set(type, true);
    //}

    //template<typename T>
    //std::shared_ptr<ComponentArray<T>> GetComponentArray()
    //{
    //    const char* typeName = typeid(T).name();

    //    assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

    //    return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
    //}
};
