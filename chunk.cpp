#include "chunk.h"


Chunk::Chunk(Signature sig, int chunkID, Spritesheet spriteSheet)
{
    this->sig = sig;
    this->chunkID = chunkID;
    this->spritesheet = spriteSheet;
}

Chunk::Chunk(Archetype archetype, int chunkID, Spritesheet spriteSheet, ComponentSizeMap& sizemap)
{
    this->arch = archetype;
    this->chunkID = chunkID;
    this->spritesheet = spriteSheet;
    addComponentArrays(archetype, sizemap);
}

void Chunk::addComponentArrays(Archetype t, ComponentSizeMap& sizemap)
{
    std::vector<ComponentType> archComponents = t.getComponentTypes();
    for (int i = 0; i < archComponents.size(); i++)
    {
        ComponentType ct = archComponents[i];
        ComponentSize cs = sizemap[ct];
        Byte* newArray = new Byte[ENTITIES_PER_CHUNK * int(cs)];
        componentArrays.emplace(ct, newArray);
    }
}

template<typename T>
void Chunk::addComponentArray()
{
    char const* typeName = typeid(T).name();
    componentArrays.insert(typeName, new T[ENTITIES_PER_CHUNK]);
}

template<typename T, typename ... args>
void Chunk::addComponentArray()
{
    char const* typeName = typeid(T).name();
    componentArrays.insert(typeName, new T[ENTITIES_PER_CHUNK]);

    addComponentArray<args...>();
}

Spritesheet Chunk::GetSpritesheet()
{
    return spritesheet;
}

int Chunk::getCurrEnts()
{
    return currEnts;
}

ChunkAddress Chunk::assignNewEntity()
{
    ChunkAddress id;
    if (currEnts == ENTITIES_PER_CHUNK)
    {
        id.chunkID = -1;
        id.index = -1;
        id.version = -1;
        return id;
    }

    for (int i = 0; i < ENTITIES_PER_CHUNK; i++)
    {
        if (entToDat[i] == -1)
        {
            id.chunkID = chunkID;
            id.index = i;
            id.version = versions[i];
            entToDat[i] = currEnts;
        }
    }

    currEnts++;

    return id;
}

void Chunk::releaseEntity(ChunkAddress id)
{
    int dataIndex = entToDat[id.index];
    int lastIndex = currEnts - 1;
    if (dataIndex != lastIndex)
    {
        // move component data with index == lastIndex into dataIndex

        // then
        int entWithLastData = datToEnt[lastIndex];
        datToEnt[dataIndex] = entWithLastData;
        entToDat[entWithLastData] = dataIndex;
    }

    entToDat[id.index] = -1;
    versions[id.index] = versions[id.index] + 1;
    currEnts--;
}

template<typename T>
T& Chunk::getComponentReference(ChunkAddress id)
{
    if (id.index >= ENTITIES_PER_CHUNK)
    {
        // throw error
    }
    int datIndex = entToDat[id.index];
    if (datIndex >= ENTITIES_PER_CHUNK)
    {
        // throw error
    }

    T* compArr = getComponentArray<T>();
    return compArr[datIndex];
}

template <typename T>
T* Chunk::getComponentArray()
{
    char const* type = typeid(T).name();
    if (componentArrays.find(type) == componentArrays.end)
    {
        // type is not in chunk component type array map
    }
    void* arr = componentArrays[type];
    T* compArr = std::static_pointer_cast<T*>(arr);
    return compArr;
}

Chunk::~Chunk()
{
    for (auto ca : componentArrays)
    {
        delete ca.second;
    }
}

template<typename T, typename ...args>
inline Chunk* createChunk(int chunkID, Archetype arch, Spritesheet spriteSheet, ComponentSizeMap& sizemap)
{
    return nullptr;
}
