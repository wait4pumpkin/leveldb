//
//  SkipList.hpp
//  leveldb
//
//  Created by Pumpkin Lee on 9/24/17.
//  Copyright © 2017 Pumpkin Lee. All rights reserved.
//

#pragma once

#include <cstddef>
#include <memory>

namespace lancer {
namespace leveldb {
namespace utils {

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL = 16>
class SkipList
{
public:
    SkipList();
    ~SkipList();
    
    SkipList(const SkipList&) = delete;
    SkipList operator=(const SkipList&) = delete;
    
    bool Search(const KeyType &&key, ValueType &&value)
    {
        return impl->Search(std::forward<const KeyType, ValueType>(key, value));
    }
    bool Insert(const KeyType &&key, const ValueType &&value)
    {
        return impl->Insert(std::forward<const KeyType, const ValueType>(key, value));
    }
    bool Remove(const KeyType &&key, ValueType &&value)
    {
        return impl->Remove(std::forward<const KeyType, ValueType>(key, value));
    }

private:
    class SkipListImpl;
    std::unique_ptr<SkipListImpl> impl;
};
    
} // namespace utils
} // namespace leveldb
} // namespace lancer
