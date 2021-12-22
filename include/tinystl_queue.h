#ifndef __TINYSTL_QUEUE_H
#define __TINYSTL_QUEUE_H

#include "tinystl_deque.h"
namespace tinystd {

template <typename _Tp, typename _Sequence> class queue;
template <typename _Tp, typename _Sequence>
bool operator==(const queue<_Tp, _Sequence> &__lhs, const queue<_Tp, _Sequence> &__rhs);
template <typename _Tp, typename _Sequence>
bool operator<(const queue<_Tp, _Sequence> &__lhs, const queue<_Tp, _Sequence> &__rhs);

template <typename _Tp, typename _Sequence = tinystd::deque<_Tp>>
class queue {
    friend bool operator==<>(const queue &, const queue &);
    friend bool operator< <>(const queue &, const queue &);
public:
    typedef _Tp value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef typename _Sequence::size_type size_type;
    typedef typename _Sequence::different_type different_type;

    queue() : _M_c() {}
    queue(const _Sequence &__seq) : _M_c(__seq) {}
    queue(const queue &__q) : queue(__q._M_c) {}
    template <typename _InputIter>
    queue(_InputIter __lhs, _InputIter __rhs) : _M_c(__lhs, __rhs) {}

    bool empty() const { return _M_c.empty(); }
    size_type size() const { return _M_c.size(); }
    reference front() { return _M_c.front(); }
    const_reference front() const { return _M_c.front(); }
    reference back() { return _M_c.back(); }
    const_reference back() const { return _M_c.back(); }
    void push(const _Tp &__data) { _M_c.push_back(__data); }
    void pop() { _M_c.pop_front(); }

protected:
    _Sequence _M_c;
};

template <typename _Tp, typename _Sequence>
bool operator==(const queue<_Tp, _Sequence> &__lhs, const queue<_Tp, _Sequence> &__rhs) {
    return __lhs._M_c == __rhs._M_c;
}

template <typename _Tp, typename _Sequence>
bool operator<(const queue<_Tp, _Sequence> &__lhs, const queue<_Tp, _Sequence> &__rhs) {
    return __lhs._M_c < __rhs._M_c;
}

}

#endif // __TINYSTL_QUEUE_H
