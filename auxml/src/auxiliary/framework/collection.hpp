#pragma once

#include <cstdint>

namespace callisto::framework
{

template<typename collection_type>
int64_t max_index(const collection_type& collection)
{
    auto collection_it = collection.begin();

    if (collection.size() == 0) return -1;

    if (collection.size() == 1) return 0;

    auto*  max_element = &collection_it[0];
    size_t max_idx     = 0;

    for (size_t i = 1; i < collection.size(); i++)
    {
        if (collection_it[i] > *max_element)
        {
            max_element = &collection_it[i];
            max_idx     = i;
        }
    }

    return max_idx;
}

} // namespace callisto::framework