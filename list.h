#ifndef LIST_H_
#define LIST_H_

#include "float.h"

// Generic Meta List : Head -> Tail, ends with Nil

struct Nil
{
	typedef Nil Head;
	typedef Nil Tail;
};

template <class Head_, class Tail_ = Nil>
struct List
{
	typedef Head_ Head;
	typedef Tail_ Tail;
};

// Computes the length of the list

template<class List>
struct Length
{
	enum {
		value = 1 + Length<typename List::Tail>::value
	};
};

template<>
struct Length<Nil> 
{
	enum {
		value = 0 
	}; 
};

// Computes the sum of all the elements of the list

template <class List, template <class> class Selector>
struct sum
{
	typedef typename add_meta<
		typename Selector<typename List::Head>::value,
		typename sum<typename List::Tail, Selector>::value>::value value;
};

template <template <class> class Selector>
struct sum<Nil, Selector>
{
	typedef float_meta<0, 0> value;
};

#endif 
