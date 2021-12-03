#include "EntityQueryParameters.h"

std::size_t EntityQueryParameters::ComponentTypesHash()
{
    std::size_t seed = compTypes.size();
    for (int i = 0; i < compTypes.size(); i++)
    {
        seed ^= compTypes[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed; 
}

std::size_t EntityQueryParameters::TagsHash()
{
    std::size_t seed = tags.size();
    for (int i = 0; i < tags.size(); i++)
    {
        seed ^= tags[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

std::size_t EntityQueryParameters::QueryParameterHash()
{
    std::size_t seed = compTypes.size() << 8;
    seed += tags.size();
    std::size_t hash = ComponentTypesHash();
    hash ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return hash;
}
