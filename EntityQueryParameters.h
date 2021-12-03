#pragma once
#include <vector>
#include "Tags.h"

class EntityQueryParameters {
    std::vector<ComponentType> compTypes;
    std::vector<Tag> tags;    
    std::size_t ComponentTypesHash();
    std::size_t TagsHash();
    std::size_t QueryParameterHash();
};
