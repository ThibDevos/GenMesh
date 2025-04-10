#ifndef MESH
#define MESH

#include <mesh/simplex.h>
#include <mesh/topology.h>
#include <mesh/geometry.h>
#include <file_functions.h>
#include <string>
#include <fstream>
#include <sstream>

template<template<int> typename T>
struct gmesh_types
{
  static std::array<int, 0> types;
};

template<>
struct gmesh_types<simplex>
{
  static constexpr std::array<int, 4> types = {15,1,2,4};
};




//T is the element type (simplices for instance), G is the geometrical dimension (1 in 1D, 2 in 2D, 3 in 3D)
//D is the highest dimension of the elements (2 for triangle or 3 for tetrahedron for instance)
template <template<int> typename T, int G, int D=G>
class mesh
{

  public:
    mesh(){;}

    void build_edges(){topo.build_edges();}

    int nb_nodes;
    int nb_cells;
  
    geometry<G> geo;
    topology<T,D> topo;
    
};

template<template<int> typename T, int D0, int G, int D>
std::array< entity<T,0>*, T<D0>::nb_sub_included[0]> vertices(mesh<T,G,D> const & M, entity<T,D0> const & E)
{
  if constexpr (D0 == D)
    return M.topo.C_CV[E.get_index()];
  else if constexpr (D0 == 1)
    return M.topo.C_EV[E.get_index()];
}
template<template<int> typename T, int D0, int G, int D>
std::vector<entity<T,0>> vertices(mesh<T,G,D> const & M)
{
  return M.topo.V;
}

template<template<int> typename T, int G, int D>
std::vector<entity<T,D>> cells(mesh<T,G,D> const & M)
{
  return M.topo.C;
}

#endif //MESH