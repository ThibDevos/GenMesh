#ifndef TOPOLOGY
#define TOPOLOGY

#include <mesh/entity.h>
#include <graph.h>
/*!
\brief The topology of the mesh is set of the different entities composing the mesh and their connectivities
*/
template<template<int> typename T, int D>
class topology
{

  template <int D0, int D1>
  using connectivity = std::vector< std::array< entity<T,D1>*, T<D0>::nb_sub_included[D1]> >;



  public: 


  std::vector<entity<T,0>> V; //!< vertices 
  std::vector<entity<T,1>> E; //!< edges
  std::vector<entity<T,D>> C; //!< cells

  
  connectivity<D,0> C_CV; //!< connectivity cells-vertices, C_CV.size() = nb cells
  connectivity<1,0> C_EV; //!< connectivity edges-vertices, C_EV.size() = nb edges
  
  /// \todo
  void build_edges() 
  {
    E.resize(V.size()); //assume that number of edges is approximately the number of vertices
    C_EV.resize(E.size());

    int index = 0;
    for(auto c : C)
    {
      int ci = c.get_index();
      
    }
  }

};



#endif