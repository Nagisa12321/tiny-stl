#ifndef TINYSTL_PAIR_H
#define TINYSTL_PAIR_H

namespace tinystd {

template <typename _Tp1, typename _Tp2> 
struct pair {
    pair(const _Tp1 &__x1, const _Tp2 &__x2)
        : first(__x1)
        , second(__x2) {}
    typedef _Tp1 first_type;
    typedef _Tp2 second_type;
    _Tp1 first;
    _Tp2 second;
};

}


#endif // TINYSTL_PAIR_H
