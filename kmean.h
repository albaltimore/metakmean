#ifndef KMEAN_H
#define KMEAN_H

#include "basic.h"
#include "list.h"
#include "float.h"

// Representation of a cluster: a centroid + points within this cluster

template <class Centroid, class List>
struct s
{
	typedef Centroid c;
	typedef List l;
};

// Computes the distance between two points

template <class point1, class point2>
struct distance
{
	typedef typename sub_meta<typename point1::x, typename point2::x>::value tmp_x;
	typedef typename sub_meta<typename point1::y, typename point2::y>::value tmp_y;
	typedef typename add_meta<	typename mult_meta<tmp_x, tmp_x>::value, 
								typename mult_meta<tmp_y, tmp_y>::value >::value value;
};

// Find the closest point in a list
// Input: point + list
// Output: find closest point in 'list' from 'point'
//         min_point: result point, value: distance between min_point and point

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

// Computes the centroid of a list of points

template <class List>
struct compute_centroid
{
	typedef typename div_meta<typename sum<List, x_selector>::value, 
		float_meta< Length<List>::value, 0 > >::value x;

	typedef typename div_meta<typename sum<List, y_selector>::value, 
		float_meta< Length<List>::value, 0 > >::value y;
};

// Extracts the first N centroids from the list

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

// Empty list of cluster by removing points in each cluster, but keeping the centroids

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

// Append the point 'pt' to the cluster's list which has 'centroid' for centroid

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

// Create a new cluster list by finding the closest centroid of each point
// and add each point to the associated cluster's point list

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

// Update centroids from the points present in each cluster

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

// Utility to compare if two lists are equal

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

// Main Class Version 1
// Inputs: ITER    Fixed number of iterations for the algorithm
//         N       Number of clusters to find
//         List    List of input points

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

// Main Class Version 2
// Inputs: 
//         N       Number of clusters to find
//         List    List of input points
// The algorithm will stop when the list of centroids/cluster doesn't change anymore.

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

#endif