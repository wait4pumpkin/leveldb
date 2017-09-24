//
//  SkipList.cpp
//  leveldb
//
//  Created by Pumpkin Lee on 9/24/17.
//  Copyright © 2017 Pumpkin Lee. All rights reserved.
//

#include "SkipList.hpp"

#include <memory>

using namespace std;
using namespace lancer::leveldb::utils;

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::SkipList() : impl(make_unique<SkipListImpl>())
{
}

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::~SkipList()
{
}

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
class SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::SkipListImpl
{
public:
    struct SkipListNode
    {
        KeyType key;
        ValueType value;
        unique_ptr<SkipListNode[]> forwards;
        
        SkipListNode(size_t level)
        {
            forwards = make_unique<SkipListNode[]>(level + 1);
        }
    };
    
    SkipListImpl();
    ~SkipListImpl();
    
    SkipListImpl(const SkipListImpl&) = delete;
    SkipListImpl operator=(const SkipListImpl&) = delete;
    
    bool Search(const KeyType &key, ValueType &value);
    bool Insert(const KeyType &key, const ValueType &value);
    bool Remove(const KeyType &key, ValueType &value);
    
private:
    unique_ptr<unique_ptr<SkipListNode>[]> header_;
    size_t level_;
};

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::SkipListImpl::SkipListImpl()
: header_(make_unique<SkipListNode[]>(MAX_LEVEL)), level_(0)
{
    for (auto n = 0; n < MAX_LEVEL; ++n)
    {
        header_[n] = make_unique<SkipListNode>(n);
    }
}

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::SkipListImpl::~SkipListImpl()
{
}

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
bool SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::SkipListImpl::Search(const KeyType &key, ValueType &value)
{
    const auto node = header_[level_];
    for (auto n = level_; n >= 0; --n)
    {
        while (node->forward[n] && Comparator(key, node->forward[n]->key) > 0)
        {
            node = node->forward[n];
        }
    }
    
    node = node->forward[0];
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
bool SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::SkipListImpl::Insert(const KeyType &key, const ValueType &value)
{
    return false;
}

template<typename KeyType, typename ValueType, typename Comparator, size_t MAX_LEVEL>
bool SkipList<KeyType, ValueType, Comparator, MAX_LEVEL>::SkipListImpl::Remove(const KeyType &key, ValueType &value)
{
    return false;
}
