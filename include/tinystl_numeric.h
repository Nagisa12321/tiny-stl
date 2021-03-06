#ifndef TINYSTL_NUMERIC_H
#define TINYSTL_NUMERIC_H
#include "tinystl_functional.h"
#include "tinystl_iterator_base.h"

namespace tinystd {

template <typename _InputIter, typename _Tp, typename _BinaryOperation>
_Tp accumulate(_InputIter __first, _InputIter __last, _Tp __init, _BinaryOperation __op) {
    while (__first != __last) 
        { __init = __op(__init, *__first++); }
    return __init;
}

template <typename _InputIter, typename _Tp>
_Tp accumulate(_InputIter __first, _InputIter __last, _Tp __init) 
    { return tinystd::accumulate(__first, __last, __init, tinystd::plus<_Tp>()); }

// Computes the differences between the second and the first of each adjacent pair of 
// elements of the range [first, last) and writes them to the range beginning at 
// d_first + 1. An unmodified copy of *first is written to *d_first.
template <typename _InputIter, typename _OutputIter, typename _BinaryOperation>
_OutputIter adjacent_difference(_InputIter __first, _InputIter __last, _OutputIter __d_first, _BinaryOperation __op) {
    if (__first == __last) 
        { return __d_first; }
    typedef typename __iterator_traits<_InputIter>::value_type __value_type;
    __value_type __acc = *__first;
    *__d_first = __acc;
    while (++__first != __last) { 
        __value_type __value = *__first;
        *++__d_first = __op(__value, __acc); 
        __acc = __value;
    }
    return ++__d_first;
}

template <typename _InputIter, typename _OutputIter>
_OutputIter adjacent_difference(_InputIter __first, _InputIter __last, _OutputIter __d_first) 
    { return tinystd::adjacent_difference(__first, __last, __d_first, 
        tinystd::minus<typename __iterator_traits<_InputIter>::value_type>()); }


template <typename _InputIter1, typename _InputIter2, 
          typename _BinaryOperation1, typename _BinaryOperation2, typename _Tp>
_Tp inner_product(_InputIter1 __first1, _InputIter1 __last1, 
                  _InputIter2 __first2, _Tp __init, 
                  _BinaryOperation1 __op1, _BinaryOperation2 __op2) {
    while (__first1 != __last1) 
        { __init = __op1(__init, __op2(*__first1++, *__first2++)); }
    return __init;
}

template <typename _InputIter1, typename _InputIter2, typename _Tp>
_Tp inner_product(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _Tp __init) 
    { return tinystd::inner_product(__first1, __last1, __first2, __init, tinystd::plus<_Tp>(), tinystd::multiplies<_Tp>()); }


template <typename _InputIter, typename _OutputIter, typename _BinaryOperation>
_OutputIter partial_sum(_InputIter __first, _InputIter __last, _OutputIter __d_first, _BinaryOperation __op) {
    if (__first == __last) 
        { return __d_first; }
    typedef typename __iterator_traits<_InputIter>::value_type __value_type;
    __value_type __sum = *__first++;
    *__d_first++ = __sum;
    while (__first != __last) {
        __sum = __op(__sum, *__first++);
        *__d_first++ = __sum;
    }
    return ++__d_first;
}

template <typename _InputIter, typename _OutputIter>
_OutputIter partial_sum(_InputIter __first, _InputIter __last, _OutputIter __d_first) 
    { return tinystd::partial_sum(__first, __last, __d_first, 
        tinystd::plus<typename __iterator_traits<_InputIter>::value_type>()); }

template <typename _ForwardIter, typename _Tp>
void iota(_ForwardIter __first, _ForwardIter __last, _Tp __value) {
    while (__first != __last) 
        { *__first++ = __value++; }
}

}
#endif // TINYSTL_NUMERIC_H
