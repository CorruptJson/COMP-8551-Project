#pragma once

#include <memory>
#include <array>
#include "ComponentManager.h"
#include "chunkManager.h"
#include "ArchetypeManager.h"
#include "Types.h"
#include "EntityQuery.h"
#include "system_manager.h"

class EntityCoordinator
{
private:
    std::unique_ptr<ComponentManager> mComponentManager;
    //std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<ChunkManager> mChunkManager;
    std::unique_ptr<ArchetypeManager> mArchetypeManager;
    std::unique_ptr<SystemManager> mSystemManager;

public:
    EntityID* testEntity;

    void Init()
    {
        // Create pointers to each manager
        mComponentManager = std::make_unique<ComponentManager>();
        //mEntityManager = std::make_unique<EntityManager>();
        mChunkManager = std::make_unique<ChunkManager>();
        mArchetypeManager = std::make_unique<ArchetypeManager>();
        //mChunkManager = std::make_unique<ProtoChunkManager>();
        mSystemManager = std::make_unique<SystemManager>();

        initializeSystemManager();
    }


    // Entity methods
    //Entity CreateEntity()
    //{
    //    return mEntityManager->CreateEntity();
    //}

    EntityID CreateEntity(Archetype arch, Spritesheet sprite)
    {
        return mChunkManager->assignNewEntity(arch, sprite, mComponentManager->mComponentSizes);
    }

    Archetype GetArchetype(std::vector<ComponentType> compTypes)
    {
        return mArchetypeManager->getArchetype(compTypes);
    }

    //template<typename T, typename... Args>
    //EntityID CreateEntityChunked(const char* spriteSheet)
    //{
    //    Signature sig;
    //    //recursiveSetSig<T,Args...>(sig);
    //    EntityID ca = mChunkManager->assignNewEntity<T,Args...>(sig, spriteSheet);
    //    return ca;
    //}

    void DestroyEntity(EntityID entity)
    {
        //mEntityManager->DestroyEntity(entity);
        //mComponentManager->EntityDestroyed(entity);
        mChunkManager->releaseEntity(entity);
    }

    // Component methods
    template<typename T>
    void RegisterComponent()
    {
        mComponentManager->RegisterComponent<T>();
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        return mComponentManager->GetComponentType<T>();
    }



    //template<typename T>
    //void AddComponent(Entity entity, T component)
    //{
    //    mComponentManager->AddComponent<T>(entity, component);

    //    auto signature = mEntityManager->GetSignature(entity);
    //    signature.set(mComponentManager->GetComponentType<T>(), true);
    //    mEntityManager->SetSignature(entity, signature);

    //    //mSystemManager->EntitySignatureChanged(entity, signature);
    //}

    //template<typename T>
    //void RemoveComponent(Entity entity)
    //{
    //    mComponentManager->RemoveComponent<T>(entity);

    //    auto signature = mEntityManager->GetSignature(entity);
    //    signature.set(mComponentManager->GetComponentType<T>(), false);
    //    mEntityManager->SetSignature(entity, signature);

    //    //mSystemManager->EntitySignatureChanged(entity, signature);
    //}

    template<typename T>
    T& GetComponent(EntityID entity)
    {
        return mChunkManager->getComponentRef<T>(entity);
    }
    
    std::unique_ptr<EntityQuery> GetEntityQuery(std::vector<ComponentType> compTypes)
    {
        return std::make_unique<EntityQuery>(compTypes, mChunkManager->allChunks);        
    }

    //template<typename T>
    //std::array<T>& GetComponentArray () {
    //    return mComponentManager->GetComponentArray<T>() -> GetComponentArray();
    //}

    // System methods
    /*
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return mSystemManager->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature signature)
    {
        mSystemManager->SetSignature<T>(signature);
    }
    */

    //template<typename... Args>
    //void getSigFromComponents(Args... args)
    //{
    //    Signature sig;
    //    recursiveSetSig(sig, args...);
    //    std::string sigString = SignatureToString(sig);
    //    std::cout << "Recursive Set Sig is: " << sigString << std::endl;
    //}

    //template<typename T>
    //void recursiveSetSig(Signature& sig)
    //{
    //    mComponentManager->SetSignatureBit<T>(sig);
    //}

    //template<typename T, typename... Args>
    //void recursiveSetSig(Signature& sig)
    //{
    //    mComponentManager->SetSignatureBit<T>(sig);
    //    recursiveSetSig<Args...>(sig);
    //}

    //std::string SignatureToString(Signature sig)
    //{
    //    std::string s;
    //    for (int i = sig.size() - 1; i >= 0; i--)
    //    {
    //        s += sig[i] ? '1' : '0';
    //    }
    //    return s;
    //}

    // test functions that list out components passed as arguments

    //template<typename T>
    //void identifyComponents()
    //{
    //    const char* typeName = typeid(T).name();
    //    ComponentType type = mComponentManager->GetComponentType<T>();

    //    std::cout << "type name: " << typeName << ", type num: " << unsigned(type) << std::endl;
    //}

    //template<typename T, typename... Args>
    //void identifyComponents()
    //{
    //    const char* typeName = typeid(T).name();
    //    ComponentType type = mComponentManager->GetComponentType<T>();

    //    std::cout << "type name: " << typeName << ", type num: " << unsigned(type) << std::endl;

    //    identifyComponents<Args...>();
    //}

    uint32_t GetEntityCount()
    {
        return mChunkManager->GetEntityCount();
    }

    void runSystemUpdates() {
        mSystemManager->runUpdates();
    }

    void initializeSystemManager() {
        mSystemManager->coordinator = this;

        mSystemManager->addSystem<TestSystem>(this);
    }
};
