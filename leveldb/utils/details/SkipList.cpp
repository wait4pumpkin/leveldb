//
//  SkipList.hpp
//  leveldb
//
//  Created by Pumpkin Lee on 9/24/17.
//  Copyright © 2017 Pumpkin Lee. All rights reserved.
//

#pragma once

#include "SkipList.hpp"

#include <cstddef>
#include <memory>
#include <random>

using namespace std;
using namespace lancer::leveldb::utils;

#define K_BRANCHING 4

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
struct SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::SkipListNode
{
    KeyType key;
    ValueType value;
    unique_ptr<unique_ptr<SkipListNode>[]> forwards;
    
    explicit SkipListNode(size_t level)
    {
        forwards = make_unique<unique_ptr<SkipListNode>[]>(level + 1);
    }
    
    SkipListNode(size_t level, KeyType&& key, ValueType&& value)
    {
        forwards = make_unique<SkipListNode[]>(level + 1);
        this->key = move(key);
        this->value = move(value);
    }
};
    
template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::SkipList()
: header_(make_unique<unique_ptr<SkipListNode>[]>(MAX_LEVEL)), level_(0), distribution_(1, K_BRANCHING)
{
    for (auto n = 0; n < MAX_LEVEL; ++n)
    {
        header_[n] = make_unique<SkipListNode>(n);
    }
}

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::~SkipList()
{
}
    
template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
bool SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::Search(const KeyType &key, ValueType &value) const
{
    const auto &node = header_[level_];
    for (auto n = level_; n >= 0; --n)
    {
        while (node->forwards[n] && Comparator(key, node->forwards[n]->key) > 0)
        {
            node = node->forward[n];
        }
    }
    
    node = node->forwards[0];
    if (node && Comparator(key, node->key) == 0)
    {
        value = node->value;
        return true;
    }
    else
    {
        return false;
    }
}

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
bool SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::Insert(const KeyType &key, const ValueType &value)
{
    const auto &node = header_[level_];
    unique_ptr<SkipListNode> memo[MAX_LEVEL];
    for (auto n = level_; n >= 0; --n)
    {
        while (node->forwards[n] && Comparator(key, node->forwards[n]->key) > 0)
        {
            node = node->forward[n];
        }
        memo[n] = node;
    }
    
    node = node->forwards[0];
    if (node && Comparator(key, node->key) == 0)
    {
        return false;
    }
    else
    {
        auto height = randomHeight();
        auto node = make_unique<SkipListNode>(height, key, value);
        for (auto n = 0; n < height; ++n)
        {
            node->forward[n].swap(memo[n]->forward[n]);
            memo[n]->forward[n] = node;
        }
        return true;
    }
}

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
bool SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::Remove(const KeyType &key, ValueType &value)
{
    const auto &node = header_[level_];
    unique_ptr<SkipListNode> memo[MAX_LEVEL];
    for (auto n = level_; n >= 0; --n)
    {
        while (node->forward[n] && Comparator(key, node->forward[n]->key) > 0)
        {
            node = node->forward[n];
        }
        memo[n] = node;
    }
    
    node = node->forward[0];
    if (node && Comparator(key, node->key) == 0)
    {
        value = move(node->value);
        for (auto n = 0; n < node->level; ++n)
        {
            memo[n]->forward[n].swap(node->forward[n]);
        }
        return true;
    }
    else
    {
        return false;
    }
}
    
template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
size_t SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::randomHeight() const
{
    auto height = 1;
    while (height < MAX_LEVEL && distribution_(generator_) == K_BRANCHING)
    {
        ++height;
    }
    return height;
}
