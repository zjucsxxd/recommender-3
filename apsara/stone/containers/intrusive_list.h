// Copyright (c) 2013, Alibaba Inc.
// All right reserved.
//
// Author: DongPing HUANG <dongping.huang@alibaba-inc.com>
// Created: 2013/08/03
// Description:

#ifndef STONE_CONTAINERS_INTRUSIVE_LIST_H
#define STONE_CONTAINERS_INTRUSIVE_LIST_H

#include <assert.h>
#include <algorithm>
#include <iterator>
#include "stone/base/uncopyable.h"
#include "stone/utility/member_to_object.h"

// GLOBAL_NOLINT(runtime/references)
namespace apsara {
namespace stone {

struct LinkNode
{
    LinkNode() : next(0) {}
    ~LinkNode()
    {
        Unlink();
    }
    void Unlink()
    {
        next = 0;
    }
    LinkNode* next;
};

template<typename T,
         LinkNode T::*M = &T::mLinkNode >
class IntrusiveList : public Uncopyable
{
    friend class iterator;
public:
    typedef IntrusiveList self;
    typedef T element_type;

    class iterator
    {
        friend class IntrusiveList;
    public:
        typedef iterator self;
        typedef T value_type;
        typedef int difference_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::forward_iterator_tag iterator_category;

        iterator() : mNode(0) {}
        bool operator==(self const& that) const
        {
            return mNode == that.mNode;
        }
        self& operator++()
        {
            if (mNode) mNode = mNode->next;
            return *this;
        }
        self operator++(int)
        {
            self tmp(*this);
            ++*this;
            return tmp;
        }
        bool operator!=(self const& that) const { return !(*this == that); }
        reference operator*() { return *MemberToObject(mNode, M); }
        pointer operator->() { return MemberToObject(mNode, M); }
    protected:
        LinkNode* mNode;

        explicit iterator(LinkNode* node) : mNode(node) {}
    };

    // Ctor and dtor
    IntrusiveList() : mHead(0), mTail(0), mCount(0) {}
    ~IntrusiveList()
    {
        clear();
    }

    // Iterators
    iterator begin()
    {
        return iterator(mHead);
    }
    iterator end()
    {
        return iterator();
    }

    // Capacity
    bool empty() const { return mCount == 0; }
    size_t size() const { return (size_t)mCount; }
    size_t max_size() const { return 0x7FFFFFFF; }
    void resize() {}

    // Element access
    T& front()
    {
        return *MemberToObject(mHead, M);
    };
    T& back()
    {
        return *MemberToObject(mTail, M);
    };

    // Modifiers
    void push_front(T& elem)
    {
        LinkNode *node = &(elem.*M);
        node->next = mHead;
        mHead = node;
        if (++mCount == 1) {
            mTail = node;
        }
    }
    void pop_front()
    {
        LinkNode* next = mHead->next;
        mHead->Unlink();
        mHead = next;
        if (--mCount == 0) {
            mTail = 0;
        }
        assert(mCount >= 0);
    }
    void push_back(T& elem)
    {
        LinkNode *node = &(elem.*M);
        if (mTail) {
            mTail->next = node;
        }
        mTail = node;
        if (++mCount == 1) {
            mHead = node;
        }
    }
    void swap(IntrusiveList& that)
    {
        std::swap(mHead, that.mHead);
        std::swap(mTail, that.mTail);
        std::swap(mCount, that.mCount);
    }
    void clear()
    {
        while (!empty()) {
            T* elem = &front();
            pop_front();
            delete elem;
        }
    }

private:
    LinkNode* mHead;
    LinkNode* mTail;
    int32_t mCount;
};

struct DLinkNode
{
    DLinkNode() : prev(this), next(this) {}
    ~DLinkNode()
    {
        Unlink();
    }
    void Unlink()
    {
        next->prev = prev;
        prev->next = next;
        next = this;
        prev = this;
    }
    DLinkNode* prev;
    DLinkNode* next;
};

template<typename T,
         DLinkNode T::*M = &T::mLinkNode,
         bool AutoDelete = true>
class IntrusiveDList : public Uncopyable
{
    friend class iterator;
public:
    typedef IntrusiveDList self;
    typedef T element_type;

    class iterator
    {
        friend class IntrusiveDList;
    public:
        typedef iterator self;
        typedef T value_type;
        typedef int difference_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::bidirectional_iterator_tag iterator_category;

        iterator() : mNode(0) {}
        bool operator==(self const& that) const
        {
            return mNode == that.mNode;
        }
        self& operator++()
        {
            if (mNode) mNode = mNode->next;
            return *this;
        }
        self& operator--()
        {
            if (mNode) mNode = mNode->prev;
            return *this;
        }
        self operator++(int)
        {
            self tmp(*this);
            ++*this;
            return tmp;
        }
        self operator--(int)
        {
            self tmp(*this);
            --*this;
            return tmp;
        }
        bool operator!=(self const& that) const { return !(*this == that); }
        reference operator*() { return *MemberToObject(mNode, M); }
        pointer operator->() { return MemberToObject(mNode, M); }
    protected:
        DLinkNode* mNode;

        explicit iterator(DLinkNode* node) : mNode(node) {}
    };

    typedef std::reverse_iterator<iterator> reverse_iterator;

    // Ctor and dtor
    IntrusiveDList() : mRoot(new DLinkNode), mCount(0) {}
    ~IntrusiveDList()
    {
        clear();
        delete mRoot;
    }

    // Iterators
    iterator begin()
    {
        return iterator(mRoot->next);
    }
    iterator end()
    {
        return iterator(mRoot);
    }
    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }
    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    // Capacity
    bool empty() const { return mCount == 0; }
    size_t size() const { return (size_t)mCount; }
    size_t max_size() const { return 0x7FFFFFFF; }
    void resize() {}

    // Element access
    T& front()
    {
        DLinkNode* node = mRoot->next;
        return *MemberToObject(node, M);
    };
    T& back()
    {
        DLinkNode* node = mRoot->prev;
        return *MemberToObject(node, M);
    };

    // Modifiers
    void push_front(T& elem)
    {
        insert(begin(), elem);
    }
    void pop_front()
    {
        DLinkNode* node = mRoot->next;
        node->Unlink();
        --mCount;
        assert(mCount >= 0);
    }
    void push_back(T& elem)
    {
        insert(end(), elem);
    }
    void pop_back()
    {
        DLinkNode* node = mRoot->prev;
        node->Unlink();
        --mCount;
        assert(mCount >= 0);
    }
    iterator insert(iterator it, T& elem)
    {
        DLinkNode* curr = it.mNode;
        DLinkNode* node = &(elem.*M);
        // link it
        node->prev = curr->prev;
        node->next = curr;
        curr->prev->next = node;
        curr->prev = node;
        ++mCount;
        return iterator(node);
    }
    iterator erase(iterator it)
    {
        if (it == end()) {
            return it;
        }
        DLinkNode* node = it.mNode;
        DLinkNode* next = node->next;
        node->Unlink();
        if (AutoDelete) {
            delete MemberToObject(node, M);
        }
        --mCount;
        assert(mCount >= 0);
        return iterator(next);
    }
    void swap(IntrusiveDList& that)
    {
        std::swap(mRoot, that.mRoot);
        std::swap(mCount, that.mCount);
    }
    void clear()
    {
        for (iterator it = begin(); it != end(); ) {
            it = erase(it);
        }
    }
    // this method is not safe, there is not an effective way to verify
    // whether a node belongs to the list
    void unlink(T& elem)
    {
        DLinkNode* node = &(elem.*M);
        node->Unlink();
        --mCount;
        assert(mCount >= 0);
    }

private:
    DLinkNode* mRoot;
    int32_t mCount;
};

} // namespace stone
} // namespace apsara

namespace std {

template<typename T, typename apsara::stone::LinkNode T::*M>
void swap(apsara::stone::IntrusiveList<T, M>& left,
          apsara::stone::IntrusiveList<T, M>& right)
{
    left.swap(right);
}

template<typename T, typename apsara::stone::DLinkNode T::*M>
void swap(apsara::stone::IntrusiveDList<T, M>& left,
          apsara::stone::IntrusiveDList<T, M>& right)
{
    left.swap(right);
}

} // namespace std

#endif // STONE_CONTAINERS_INTRUSIVE_LIST_H
