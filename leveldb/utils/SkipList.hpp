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
#include <random>

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
    
    bool Search(const KeyType &key, ValueType &value) const;
    bool Insert(const KeyType &key, const ValueType &value);
    bool Remove(const KeyType &key, ValueType &value);

private:
    struct SkipListNode;

    size_t randomHeight() const;
    
    std::unique_ptr<std::unique_ptr<SkipListNode>[]> header_;
    size_t level_;
    
    std::default_random_engine generator_;
    std::uniform_int_distribution<int> distribution_;
};

#include "details/SkipList.cpp"

} // namespace utils
} // namespace leveldb
} // namespace lancer
