#include "basic.h"
#include "list.h"
#include "float.h"

template <class Centroid, class List>
struct s
{
	typedef Centroid c;
	typedef List l;
};

template <class point1, class point2>
struct distance
{
	typedef typename sub_meta<typename point1::x, typename point2::x>::value tmp_x;
	typedef typename sub_meta<typename point1::y, typename point2::y>::value tmp_y;
	typedef typename add_meta<	typename mult_meta<tmp_x, tmp_x>::value, 
								typename mult_meta<tmp_y, tmp_y>::value >::value value;
};

template <class init, class m_point, class point, class list_s>
struct mini_aux
{
	typedef typename list_s::Head head;
	typedef typename distance<point, typename head::c>::value tmp_val;
	typedef typename IF< 
		inf_meta<tmp_val, init>::value,
		mini_aux<tmp_val, typename head::c, point, typename list_s::Tail>, 
		mini_aux<init, m_point, point, typename list_s::Tail> >::value tmp;

	typedef typename tmp::value value;
	typedef typename tmp::min_point min_point;
};

template <class init, class m_point, class point>
struct mini_aux<init, m_point, point, Nil>
{
	typedef init value;
	typedef m_point min_point;
};

template <class point, class list_s>
struct mini
{
	typedef typename list_s::Head head;
	typedef mini_aux<
		typename distance<point, typename head::c>::value,
		typename head::c,
		point,
		typename list_s::Tail> tmp;

	typedef typename tmp::value value;
	typedef typename tmp::min_point min_point;
};

template <class List>
struct compute_centroid
{
	typedef typename div_meta<typename sum<List, x_selector>::value, 
		float_meta< Length<List>::value, 0 > >::value x;

	typedef typename div_meta<typename sum<List, y_selector>::value, 
		float_meta< Length<List>::value, 0 > >::value y;
};

template <class list, int N>
struct extract_centroid
{
	typedef List<typename list::Head, typename extract_centroid<typename list::Tail, N - 1>::value> value;
};

template <class list>
struct extract_centroid<list, 0>
{
	typedef Nil value;
};

template <int N>
struct extract_centroid<Nil, N>
{
	typedef Nil value;
};

template <class list_c>
struct get_empty_list_s
{
	typedef typename get_empty_list_s<typename list_c::Tail>::value new_tail;
	typedef List< s<typename list_c::Head, Nil>, new_tail > value;
};

template <>
struct get_empty_list_s<Nil>
{
	typedef Nil value;
};

template <class pt, class centroid, class head_list_s, class list_s>
struct append_list
{
	typedef typename list_s::Tail tail;
	typedef typename tail::Head tail_head;
	typedef typename append_list<pt, centroid, typename tail_head::c, tail>::value new_tail;
	typedef List<typename list_s::Head, new_tail> value;
};

template <class pt, class centroid, class list_s>
struct append_list<pt, centroid, centroid, list_s>
{
	typedef typename list_s::Head head;
	typedef s< centroid, List<pt, typename head::l> > new_head;
	typedef List< new_head, typename list_s::Tail > value;
};

template <class list_pt, class list_s>
struct create_list_s
{
	typedef typename mini<typename list_pt::Head, list_s>::min_point min_centroid;
	typedef typename list_s::Head head;
	typedef typename append_list<typename list_pt::Head, min_centroid, typename head::c, list_s>::value new_list_s;
	typedef typename create_list_s<typename list_pt::Tail, new_list_s>::value value;
};

template <class list_s>
struct create_list_s<Nil, list_s>
{
	typedef list_s value;
};

template <class list_s_param>
struct update_centroids
{
	typedef typename list_s_param::Head head;
	typedef compute_centroid<typename head::l> new_centroid;
	typedef s< point<typename new_centroid::x, typename new_centroid::y> , Nil> new_head;
	typedef List<new_head, typename update_centroids<typename list_s_param::Tail>::list_s > list_s;
};

template <>
struct update_centroids<Nil>
{
	typedef Nil list_s;
};

template <class List1, class List2>
struct equal_list_s
{
	typedef typename List1::Head head1;
	typedef typename List2::Head head2;

	enum {
		value = IF_VAL< equal_point<typename head1::c, typename head2::c>::value,
						equal_list_s<typename List1::Tail, typename List2::Tail>::value, false>::value
	};
};

template <>
struct equal_list_s<Nil, Nil>
{
	enum {
		value = true
	};
};

template <int ITER, int N, class List>
struct kmean_iter
{
	typedef typename kmean_iter<ITER - 1, N, List>::list_s prev_list_s;
	typedef typename update_centroids<prev_list_s>::list_s updated_list_s;
	typedef typename create_list_s<List, updated_list_s>::value list_s;	
};

template <int N, class List>
struct kmean_iter<0, N, List>
{
	typedef typename extract_centroid<List, N>::value list_centroid;
	typedef typename get_empty_list_s<list_centroid>::value empty_list_s;
	typedef typename create_list_s<List, empty_list_s>::value list_s;
};

template <class List, class list_s, int ITER, bool done>
struct kmean_aux
{
	typedef typename update_centroids<list_s>::list_s updated_list_s;
	typedef typename create_list_s<List, updated_list_s>::value new_list_s;
	typedef kmean_aux<	List, new_list_s, ITER + 1, 
								equal_list_s<list_s, new_list_s>::value > tmp;

	typedef typename tmp::value value;
	enum {
		iter = tmp::iter
	};
};

template <class List, class list_s, int ITER>
struct kmean_aux<List, list_s, ITER, true>
{
	typedef list_s value;
	enum {
		iter = ITER
	};
};

template <int N, class List>
struct kmean
{
	typedef typename extract_centroid<List, N>::value list_centroid;
	typedef typename get_empty_list_s<list_centroid>::value empty_list_s;
	typedef typename create_list_s<List, empty_list_s>::value list_s;
	typedef kmean_aux<List, list_s, 0, false> tmp;

	typedef typename tmp::value value;
	enum {
		iter = tmp::iter
	};
};

#include <iostream>

template <class List>
struct print_list
{
	typedef typename List::Head cur;

	static void exec()
	{
		std::cout	
			<< "(" << cur::x::i << "." << cur::x::f
			<< "," << cur::y::i << "." << cur::y::f 
			<< ")" << std::endl;
		print_list<typename List::Tail>::exec();
	}
};

template <>
struct print_list<Nil>
{
	static void exec() {}
};

template <class List>
struct print_list_s
{
	typedef typename List::Head cur;
	typedef typename cur::c centroid;
	typedef typename cur::l cluster;

	static void exec()
	{
		std::cout	
			<< "(" << centroid::x::i << "." << centroid::x::f
			<< "," << centroid::y::i << "." << centroid::y::f 
			<< ") --> " << std::endl;

		print_list<cluster>::exec();
		print_list_s<typename List::Tail>::exec();
	}
};

template <>
struct print_list_s<Nil>
{
	static void exec() {}
};


int main()
{
	typedef List< point<f(1, 0), f(1, 0)>, List< point<f(2, 0), f(1, 0)>, 
			List< point<f(4, 0), f(3, 0)>, List< point<f(5, 0), f(4, 0)>, Nil> > > > my_list;
	print_list<my_list>::exec();

	typedef kmean_iter<3, 2, my_list>::list_s res;
	print_list_s<res>::exec();

	std::cout << "------" << std::endl;

	typedef kmean<2, my_list> tmp;
	std::cout << "Iter: " << tmp::iter << std::endl;
	print_list_s<tmp::value>::exec();

	return 0;
}
