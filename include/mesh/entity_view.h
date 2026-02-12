#ifndef MESH_ENTITY
#define MESH_ENTITY

#include <numeric>
#include <cassert>
#include <mesh/mesh.h>
#include <mesh/topology/shape.h>


/*!
\brief 
*/
template <typename Mesh, int D> 
class entity_view
{
  public:
    entity_view(Mesh * M_, size_t i) : M(M_), index_(i) {}
    auto get_incidence(int d){return M->call_the_right_incidence_tab;} //return a list of indices
    auto get_adjacent(int depth=1); //return neighbours (list of indices)
    size_t index(){return index_;}
  private:
    Mesh* M;
    size_t index_;
  
};

#endif //ENTITY