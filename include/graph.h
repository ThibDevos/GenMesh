#ifndef GRAPH
#define GRAPH



template<template<int> typename T, int D, int D0>
class graph
{



  public:

    graph(entity<T,D> E)
    {
      ;
    }


    std::array<entity<T,D0>*, T<D>::nb_sub_included[D0]> entities;
};

#endif