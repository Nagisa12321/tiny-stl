#ifndef TINYSTL_NUMERIC_H
#define TINYSTL_NUMERIC_H
#include "tinystl_functional.h"

namespace tinystd {

template <typename _InputIter, typename _Tp, typename _BinaryOperation>
_Tp accumulate(_InputIter __first, _InputIter __last, _Tp __init, const _BinaryOperation &__op) {
    while (__first != __last) 
        { __init = __op(__init, *__first++); }
    return __init;
}

template <typename _InputIter, typename _Tp>
_Tp accumulate(_InputIter __first, _InputIter __last, _Tp __init) 
    { return tinystd::accumulate(__first, __last, __init, tinystd::plus<_Tp>()); }

}

#endif // TINYSTL_NUMERIC_H
