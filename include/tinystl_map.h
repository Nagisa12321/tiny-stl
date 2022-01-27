#ifndef __TINYSTL_MAP
#define __TINYSTL_MAP
#include "tinystl_alloc.h"
#include "tinystl_functional.h"
#include "tinystl_pair.h"
#include "tinystl_tree.h"

// for list init... 
#include <initializer_list>

namespace tinystd {

template <typename _Key, 
          typename _Value, 
          typename _Compare = tinystd::less<_Key>,
          typename _Alloc = __DEFAULT_ALLOCATOR>
class map {
public:
    typedef _Key key_type;
    typedef _Value data_type;
    typedef _Value mapped_type;
    typedef tinystd::pair<_Key, _Value> value_type;
    typedef _Compare key_compare;

    class value_compare {
    private:
        _Compare _M_comp;
        value_compare(const _Compare &__comp)
            : _M_comp(__comp) {}
    public:
        bool operator()(const value_type &__lhs, const value_type &__rhs) const
            { return _M_comp(__lhs.first, __rhs.first); }
    };

private:
    typedef __avl_tree<key_type, value_type, tinystd::__select1st<value_type>, key_compare> __rep_type;
    __rep_type _M_tree;

public:
    typedef typename __rep_type::pointer pointer;
    typedef typename __rep_type::const_pointer const_pointer;
    typedef typename __rep_type::reference reference;
    typedef typename __rep_type::const_reference const_reference;
    typedef typename __rep_type::iterator iterator;
    typedef typename __rep_type::const_iterator const_iterator;
    typedef typename __rep_type::size_type size_type;
    typedef typename __rep_type::difference_type difference_type;

    map()
        : _M_tree(_Compare()) {}
    map(const _Compare __comp)
        : _M_tree(__comp) {}
    template <typename _InputIter>
    map(_InputIter __first, _InputIter __last)
        : _M_tree(_Compare()) {
        _M_tree.insert_unique(__first, __last);
    }
    template <typename _InputIter>
    map(_InputIter __first, _InputIter __last, const _Compare &__comp)
        : _M_tree(__comp) {
        _M_tree.insert_unique(__first, __last);
    }
    map(const map &__other)
        : _M_tree(__other._M_tree) {}
    map(const std::initializer_list<value_type> &__li)
        : _M_tree(_Compare()) {
        _M_tree.insert_unique(__li.begin(), __li.end());
    }
    map(const std::initializer_list<value_type> &__li, const _Compare &__comp)
        : _M_tree(__comp) {
        _M_tree.insert_unique(__li.begin(), __li.end());
    }

    data_type &operator[](const key_type &__k) {
        tinystd::pair<iterator, bool> __f = _M_tree.find({__k, data_type()});
        if (__f.second) return __f.first->second;
        else {
            return (_M_tree.insert_equal({__k, data_type()}))->second;

        }
    }

    iterator begin()
        { return _M_tree.begin(); }
    const_iterator begin() const
        { return _M_tree.begin(); }
    iterator end()
        { return _M_tree.end(); }
    const_iterator end() const
        { return _M_tree.end(); }
    const_iterator cbegin() const
        { return _M_tree.begin(); }
    const_iterator cend() const
        { return _M_tree.cend(); }
    size_type size() const
        { return _M_tree.size(); }
    size_type max_size() const 
        { return size_type(-1); }
    void clear()
        { _M_tree.clear(); }
    bool empty() const
        { return _M_tree.empty(); }
    void swap(map &__other)
        { _M_tree.swap(__other._M_tree); }
    key_compare key_comp() const
        { return _M_tree.key_comp(); }
    iterator upper_bound(const key_type &__key) const
        { return _M_tree.upper_bound(__key); }
    iterator lower_bound(const key_type &__key) const
        { return _M_tree.lower_bound(__key); }
    iterator find(const key_type &__key) const { 
        tinystd::pair<iterator, bool> __f = _M_tree.find({__key, data_type()}); 
        if (__f.second) return __f.first;
        else return const_cast<map *>(this)->end();
    }
    tinystd::pair<iterator, bool> insert(const value_type &__val)
        { return _M_tree.insert_unique(__val); }
    void erase(iterator __pos) 
        { _M_tree.erase(*__pos); }
};

}

#endif 
