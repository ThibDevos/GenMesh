#ifndef MESH
#define MESH

#include <string>
#include <fstream>
#include <sstream>

#include <mesh/topology/topology.h>
#include <mesh/geometry/geometry.h>
#include <mesh/entity_view.h>
#include <file_functions.h>



template<typename Mesh, size_t D>
struct mesh_iterator
{
  using iterator_category = std::forward_iterator_tag;
  using difference_type   = std::ptrdiff_t;
  using value_type        = entity_view<Mesh,D> ;
  using pointer           = value_type*;  
  using reference         = value_type&;

  mesh_iterator(Mesh * M_, int i) : M(M_), idx(i) {}

  value_type operator*() const { return entity_view<Mesh,D>(M, idx);}
  pointer operator->() { return entity_view<Mesh,D>(M, idx); }

  // Prefix increment
  mesh_iterator &operator++()
  {
    idx++;
    return *this;
  }

  // Postfix increment
  mesh_iterator operator++(int)
  {
    mesh_iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator==(const mesh_iterator &a, const mesh_iterator &b) { return a.idx == b.idx; };
  friend bool operator!=(const mesh_iterator &a, const mesh_iterator &b) { return a.idx != b.idx; };


private:
  Mesh* M;
  size_t idx;
};

template<typename Mesh, size_t D>
struct mesh_range
{

  mesh_range(Mesh* M_, size_t begin, size_t end) : M(M_), begin_idx(begin), end_idx(end) {}
  mesh_iterator<Mesh, D> begin(){return mesh_iterator<Mesh ,D>(M, begin_idx);}
  mesh_iterator<Mesh, D> end(){return mesh_iterator<Mesh, D>(M, end_idx);}

  private:
    Mesh* M;
    size_t begin_idx;
    size_t end_idx;
};

/*!
\brief A mesh is a topology with a geometry

G is the geometrical dimension (1 in 1D, 2 in 2D, 3 in 3D)
D is the highest dimension of the elements (2 for triangle or 3 for tetrahedron for instance)
*/
template <int G, int D=G>
class mesh
{

  public:
    mesh(){;}

    static constexpr int dim_topo = D;
    static constexpr int dim_geo  = G;

    geometry<G> geo;
    topology<D> topo;

    //iterator on Entity<Mesh,d,G> which allows to loop on the different entities of dim d

    
    mesh_range<mesh<G, D>, D> cells(){return mesh_range<mesh<G, D>, D>(this, 0, topo.nb_cells());}
    mesh_range<mesh<G, D>, D-1> faces(){return mesh_range<mesh<G, D>, D-1>(this, 0, topo.nb_faces());}
    mesh_range<mesh<G, D>, 1> edges(){return mesh_range<mesh<G, D>, 1>(this, 0, topo.nb_edges());}
    mesh_range<mesh<G, D>, 0> vertices(){return mesh_range<mesh<G, D>, 0>(this, 0, topo.nb_vertices());}


};


#endif //MESH