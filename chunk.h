#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <iostream>
#include <algorithm>
#include "Types.h"
#include "Archetype.h"
#include "ComponentManager.h"
#include "Tags.h"
#include "PhysicsComponent.h"
#include "ISubject.h"

const int ENTITIES_PER_CHUNK = 16;

// a chunk is an object pool of entities
// entities exist as a slot in a group of component arrays
// therefore, a chunk is a collection of component arrays

// chunks only store one "archetype" of entity
// meaning only entities with the same combination of components are stored in the same chunk

// a chunk may have any number of component arrays of different types
// component data is stored via unsigned chars (which are bytes)
// the data is cast to the appropriate component type when used

// chunks are also associated with a sprite sheet and a collection of tags
// all entities in the chunk have the same sprite sheet and the same collection of tags

// chunks can delete (release) entities by flagging the data index as "free" with -1 in the entToDat array
// the version array is used to keep track of which "reused" iteration a data index is on

class Chunk
{
    friend class ChunkManager;

private:
    int chunkID = -1;
    Archetype arch;
    std::vector<Tag> tags;
    std::string spritesheet;
    int currEnts = 0;
    std::unordered_map<ComponentType, Byte*> componentArrays{};
    int versions[ENTITIES_PER_CHUNK];
    int entToDat[ENTITIES_PER_CHUNK];
    bool deleteEnt[ENTITIES_PER_CHUNK];
    bool entitiesToDelete = false;
    bool hasPhysics;

    Chunk(Archetype archetype, int chunkID, std::string spriteSheet, std::vector<Tag> tags, ComponentSizeMap& sizemap);

    void addComponentArrays(Archetype t, ComponentSizeMap& sizemap);
    void releaseEntity(int datIndex);

    template<typename T>
    void addComponentArray()
    {
        char const* typeName = typeid(T).name();
        componentArrays.insert(typeName, new T[ENTITIES_PER_CHUNK]);
    }

    // used during chunk construction, adds a new component data array to this chunk
    template<typename T, typename ... args>
    void addComponentArray()
    {
        char const* typeName = typeid(T).name();
        componentArrays.insert(typeName, new T[ENTITIES_PER_CHUNK]);

        addComponentArray<args...>();
    }

public:

    Chunk() = delete;


    std::string GetSpritesheet();

    int getCurrEntCount();

    EntityID assignNewEntity();

    void releaseEntity(EntityID id);

    template<typename T>
    T& getComponentReference(EntityID id)
    {
        if (versions[id.index] != id.version)
        {
            std::cerr << "trying to get component for entity that no longer exists?" << std::endl;
            throw "trying to get component for entity that no longer exists?";
        }
        if (id.index >= ENTITIES_PER_CHUNK)
        {
            // throw error
            throw "getComponentReference: entity id index too large";
        }
        int datIndex = entToDat[id.index];
        if (datIndex >= ENTITIES_PER_CHUNK)
        {
            // throw error
            throw "getComponentReference: entity data index too large";
        }

        T* compArr = getComponentArray<T>();
        return compArr[datIndex];
    }
    
    template<typename T>
    T* getComponentPointer(EntityID id)
    {
        if (versions[id.index] != id.version)
        {
            std::cerr << "trying to get component for entity that no longer exists?" << std::endl;
            throw "trying to get component for entity that no longer exists?";
        }
        if (id.index >= ENTITIES_PER_CHUNK)
        {
            // throw error
            throw "getComponentReference: entity id index too large";
        }
        int datIndex = entToDat[id.index];
        if (datIndex >= ENTITIES_PER_CHUNK)
        {
            // throw error
            throw "getComponentReference: entity data index too large";
        }

        T* compArr = getComponentArray<T>();
        return compArr + datIndex;
    }

    template <typename T>
    T* getComponentArray()
    {
        ComponentType type = ComponentManager::GetComponentType<T>();
        auto find = componentArrays.find(type);
        if ( find  == componentArrays.end())
        {
            // type is not in chunk component type array map
            throw "getComponentArray: component type is not in chunk component type array map";
        }
        Byte* arr = find->second;
        //T* compArr = std::static_cast<T*>(arr);
        T* compArr = (T*)arr;
        return compArr;
    }

    Archetype& getArchetype();

    bool hasComponentType(ComponentType type);

    bool hasTag(Tag tag);

    std::vector<Tag>& getAllTags();

    EntityID entityAtComponentIndex(int i);

    bool doesEntityExist(EntityID id);
    bool isDataIndexActive(int i);

    void releaseAllEntities();
    void scheduleAllEntitiesToDelete();
    void flagEntToDelete(EntityID id);
    void releaseFlaggedEntities(ISubject& subject);


    ~Chunk();
};


