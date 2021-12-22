#ifndef __TINYSTL_STACK_H
#define __TINYSTL_STACK_H

#include "tinystl_deque.h"
namespace tinystd {

template <typename _Tp, typename _Sequence  > class stack;
template <typename _Tp, typename _Sequence>
bool operator==(const stack<_Tp, _Sequence> &__lhs, const stack<_Tp, _Sequence> &__rhs);
template <typename _Tp, typename _Sequence>
bool operator<(const stack<_Tp, _Sequence> &__lhs, const stack<_Tp, _Sequence> &__rhs);

template <typename _Tp, typename _Sequence = tinystd::deque<_Tp>>
class stack {
    friend bool operator==<>(const stack &, const stack &);
    friend bool operator< <>(const stack &, const stack &);
public:
    typedef _Tp value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef typename _Sequence::size_type size_type;
    typedef typename _Sequence::different_type different_type;

    bool empty() const { return _M_c.empty(); }
    size_type size() const { return _M_c.size(); }
    reference top() { return _M_c.back(); }
    const_reference top() const { return _M_c.back(); }
    void push(const _Tp &__data) { _M_c.push_back(__data); }
    void pop() { _M_c.pop_back(); }
protected:
    _Sequence _M_c;
};

template <typename _Tp, typename _Sequence>
bool operator==(const stack<_Tp, _Sequence> &__lhs, const stack<_Tp, _Sequence> &__rhs) {
    return __lhs._M_c == __rhs._M_c;
}

template <typename _Tp, typename _Sequence>
bool operator<(const stack<_Tp, _Sequence> &__lhs, const stack<_Tp, _Sequence> &__rhs) {
    return __lhs._M_c < __rhs._M_c;
}

} // namespace tinystd

#endif // __TINYSTL_STACK_H
