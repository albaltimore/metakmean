#ifndef FLOAT_H_
#define FLOAT_H_

#include "basic.h"

// Float representation: Integer part + Floating part
// Ex: 4.78 -> i=4, f=78

template <int INT, int FLT>
struct float_meta
{
	enum {
		i = INT,
		f = FLT
	};
};

#define f(A, B) float_meta<A, B>

// Adds two float numbers

template <class F1, class F2>
struct add_meta
{
	typedef float_meta<
		F1::i + F2::i + (F1::f + F2::f) / 100 ,
		(F1::f + F2::f) % 100> value;
};

// Divides two float numbers

template <class F1, class F2>
struct div_meta
{
	typedef float_meta<
		static_cast<int>((F1::i * 100 + F1::f) / (F2::i * 100 + F2::f)),
		static_cast<int>(((F1::i * 100.0 + F1::f) / (F2::i * 100.0 + F2::f)) * 100.0 - 
			(F1::i * 100 + F1::f) / (F2::i * 100 + F2::f) * 100)> value;
};

// Multiplies two float numbers

template <class F1, class F2>
struct mult_meta
{
	typedef float_meta<
		static_cast<int>((F1::i + F1::f / 100.f) * (F2::i + F2::f / 100.f)),
		static_cast<int>(((F1::i + F1::f / 100.f) *
			(F2::i + F2::f / 100.f)) * 100.f -
		static_cast<int>((F1::i + F1::f / 100.f) *
			(F2::i + F2::f / 100.f)) * 100)> value;
};

// Subtracts two float numbers

template <class F1, class F2>
struct sub_meta
{
	typedef float_meta<
		static_cast<int>((F1::i + F1::f / 100.f) - (F2::i + F2::f / 100.f)),
		static_cast<int>(((F1::i + F1::f / 100.f) -
			(F2::i + F2::f / 100.f)) * 100.f -
		static_cast<int>((F1::i + F1::f / 100.f) -
			(F2::i + F2::f / 100.f)) * 100)> value;
};

// Operator '<' for float numbers

template <class F1, class F2>
struct inf_meta
{
	enum {
		value = (F1::i * 100 + F1::f) < (F2::i * 100 + F2::f)
	};
};

// Operator '==' for float numbers

template <class F1, class F2>
struct equal_float
{
	enum {
		value = (static_cast<int>(F1::i) == static_cast<int>(F2::i)) && (static_cast<int>(F1::f) == static_cast<int>(F2::f))
	};
};

#endif
