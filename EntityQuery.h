#pragma once
#include <iterator>
#include <cstddef>
#include <vector>
#include "Types.h"

template <typename a = void, typename b = void, typename c = void, typename d = void, typename e = void,
    typename f = void, typename g = void, typename h = void, typename i = void, typename j = void>
class EntityQuery
{
public:
    // entity count
    int entityCount;
    // entities per each chunk
    
    // range in each chunk


    // components query IDs
    std::vector<ComponentType> compTypes;

    //struct Iterator
    //{
    //    using iterator_category = std::forward_iterator_tag;
    //    using difference_type = std::ptrdiff_t;
    //    using value_type = int;
    //    using pointer = int*;
    //    using reference = int&;
    //};
};
