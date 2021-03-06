#ifndef BASIC_H_
#define BASIC_H_

#include "float.h"

// Point structure - (x, y) coordinates

template <class X, class Y>
struct point
{
    typedef X x;
    typedef Y y;
};

// Points comparison

template <class Pt1, class Pt2>
struct equal_point
{
    enum {
        value = equal_float<typename Pt1::x, typename Pt2::x>::value &&
                equal_float<typename Pt1::y, typename Pt2::y>::value
    };
};

// Utility structures to get x or y coordinates from a point

template <class point>
struct x_selector
{
    typedef typename point::x value;
};

template <class point>
struct y_selector
{
    typedef typename point::y value;
};

// Utility structures for a generic IF

template <bool cond, class Then, class Else>
struct IF
{
    typedef Then value;
};

template <class Then, class Else>
struct IF<false, Then, Else> 
{
    typedef Else value;
};

template <bool cond, bool Then, bool Else>
struct IF_VAL
{
    enum {
        value = Then
    };
};

template <bool Then, bool Else>
struct IF_VAL<false, Then, Else> 
{
    enum {
        value = Else
    };
};


#endif
