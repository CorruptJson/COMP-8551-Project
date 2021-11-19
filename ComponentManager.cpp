#include "ComponentManager.h"

ComponentType ComponentManager::typeCounter = 0;

ComponentManager* ComponentManager::main = NULL;

ComponentManager::ComponentManager()
{
    main = this;
}

ComponentSize ComponentManager::GetComponentSize(ComponentType t)
{
    return main->getComponentSize(t);
}

ComponentSize ComponentManager::getComponentSize(ComponentType t)
{
    auto find = mComponentSizes.find(t);
    if (find == mComponentSizes.end())
    {
        throw "cannot find component size: component not registered?";
    }
    else
    {
        return find->second;
    }    
}

//template<typename T>
//void ComponentManager::RegisterComponent()
//{
//    const char* typeName = typeid(T).name();
//
//    assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");
//
//    mComponentTypes.insert({ typeName, mNextComponentType });
//    mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
//
//    Signature sig;
//    //ComponentInfo info(mNextComponentType);
//
//    ++mNextComponentType;
//}
//
//template<typename T>
//ComponentType ComponentManager::GetComponentType()
//{
//    static const ComponentType typeID = typeCounter++;
//    return typeID;
//}
//
//template<typename T>
//void ComponentManager::AddComponent(Entity entity, T component)
//{
//    GetComponentArray<T>()->InsertData(entity, component);
//}
//
//template<typename T>
//void ComponentManager::RemoveComponent(Entity entity)
//{
//    GetComponentArray<T>()->RemoveData(entity);
//}
//
//template<typename T>
//T& ComponentManager::GetComponent(Entity entity)
//{
//    return GetComponentArray<T>()->GetData(entity);
//}
//
//template<typename T>
//std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray()
//{
//    const char* typeName = typeid(T).name();
//
//    assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");
//
//    return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
//}
