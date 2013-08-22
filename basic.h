#ifndef BASIC_H_
# define BASIC_H_

# include "float.h"

template <class X, class Y>
struct point
{
	typedef X x;
	typedef Y y;
};

template <class Pt1, class Pt2>
struct equal_point
{
	enum {
		value = equal_float<typename Pt1::x, typename Pt2::x>::value &&
				equal_float<typename Pt1::y, typename Pt2::y>::value
	};
};

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