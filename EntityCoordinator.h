#pragma once

#include <memory>
#include <array>
#include "ComponentManager.h"
#include "EntityManager.h"
#include "system_manager.h"
#include "chunkManager.h"

class EntityCoordinator
{
public:
    Entity testEntity;

    void Init()
    {
        // Create pointers to each manager
        mComponentManager = std::make_unique<ComponentManager>();
        mEntityManager = std::make_unique<EntityManager>();
        //mChunkManager = std::make_unique<ProtoChunkManager>();
        mSystemManager = std::make_unique<SystemManager>();

        initializeSystemManager();
    }


    // Entity methods
    Entity CreateEntity()
    {
        return mEntityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity)
    {
        mEntityManager->DestroyEntity(entity);

        mComponentManager->EntityDestroyed(entity);


    }

    // Component methods
    template<typename T>
    void RegisterComponent()
    {
        mComponentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component)
    {
        mComponentManager->AddComponent<T>(entity, component);

        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), true);
        mEntityManager->SetSignature(entity, signature);

        //mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    void RemoveComponent(Entity entity)
    {
        mComponentManager->RemoveComponent<T>(entity);

        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), false);
        mEntityManager->SetSignature(entity, signature);

        //mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& GetComponent(Entity entity)
    {
        return mComponentManager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        return mComponentManager->GetComponentType<T>();
    }


    template<typename T>
    std::array<T, MAX_ENTITIES>& GetComponentArray () {
        return mComponentManager->GetComponentArray<T>() -> GetComponentArray();
    }

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
        mComponentManager->SetSignatureBit(component, sig);
    }

    template<typename T, typename... Args>
    void recursiveSetSig(Signature& sig, T component, Args... args)
    {
        mComponentManager->SetSignatureBit(component, sig);
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
        ComponentType type = mComponentManager->GetComponentType<T>();

        std::cout << "type name: " << typeName << ", type num: " << unsigned(type) << std::endl;
    }

    template<typename T, typename... Args>
    void identifyComponents(T component, Args... args)
    {
        const char* typeName = typeid(component).name();
        ComponentType type = mComponentManager->GetComponentType<T>();

        std::cout << "type name: " << typeName << ", type num: " << unsigned(type) << std::endl;

        identifyComponents(args...);
    }

    ComponentManager& GetComponentManager()
    {
        return *mComponentManager;
    }

    uint32_t GetEntityCount()
    {
        return mEntityManager->GetEntityCount();
    }

    void runSystemUpdates() {
        preUpdate();
        update();
        postUpdate();
    }

    void preUpdate() {
        for (int i = 0; i < mSystemManager->getSystems().size(); i++) {
            mSystemManager->getSystems()[i]->preUpdate();
        }
    }

    void update() {
        for (int i = 0; i < mSystemManager->getSystems().size(); i++) {
            mSystemManager->getSystems()[i]->update();
        }
    }

    void postUpdate() {
        for (int i = 0; i < mSystemManager->getSystems().size(); i++) {
            mSystemManager->getSystems()[i]->postUpdate();
        }
    }

    void initializeSystemManager() {
        //TestSystem* ts = new TestSystem{ this, 1 }
        mSystemManager->addSystem<TestSystem>();
    }

private:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<SystemManager> mSystemManager;
    //std::unique_ptr<ProtoChunkManager> mChunkManager;
};
