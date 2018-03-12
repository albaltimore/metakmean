#include <iostream>
#include "kmean.h"

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
    // Input List
    typedef List< point<f(1, 0), f(1, 0)>, List< point<f(2, 0), f(1, 0)>,
            List< point<f(4, 0), f(3, 0)>, List< point<f(5, 0), f(4, 0)>, Nil> > > > my_list;
    print_list<my_list>::exec();

    // Version 1: find 2 clusters in the input list with 3 iterations
    typedef kmean_iter<3, 2, my_list>::list_s res;
    print_list_s<res>::exec();

    std::cout << "------" << std::endl;

    // Version 2: find 2 clusters in the input list
    typedef kmean<2, my_list> tmp;
    std::cout << "Iter: " << tmp::iter << std::endl;
    print_list_s<tmp::value>::exec();

    // Instead of printing the result list
    // The following line will cause a compilation error
    // showing the result
    tmp::value::Wat;

    return 0;
}
