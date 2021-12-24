#ifndef __TINYSTL_ITERATOR_H
#define __TINYSTL_ITERATOR_H

#include "tinystl_algo.h"
#include "tinystl_iterator_base.h"
namespace tinystd {

template <typename _Iterator>
_Iterator next(_Iterator __it, typename __iterator_traits<_Iterator>::difference_type __d = 1) {
    _Iterator __res = __it;
    tinystd::advance(__res, __d);
    return __res;
}

}

#endif // __TINYSTL_ITERATOR_H
