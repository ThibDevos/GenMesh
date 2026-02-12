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
struct Iterator
{
  using iterator_category = std::forward_iterator_tag;
  using difference_type   = std::ptrdiff_t;
  using value_type        = entity_view<Mesh,D> ;
  using pointer           = value_type*;  
  using reference         = value_type&;

  Iterator(Mesh * M_, int i) : M(M_), idx(i) {}

  value_type operator*() const { return entity_view<Mesh,D>(M, idx);}
  pointer operator->() { return entity_view<Mesh,D>(M, idx);; }

  // Prefix increment
  Iterator &operator++()
  {
    idx++;
    return *this;
  }

  // Postfix increment
  Iterator operator++(int)
  {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator==(const Iterator &a, const Iterator &b) { return a.idx == b.idx; };
  friend bool operator!=(const Iterator &a, const Iterator &b) { return a.idx != b.idx; };


private:
  Mesh* M;
  size_t idx;
};

template<typename Mesh, size_t D>
struct Iterator_range
{

  Iterator_range(Mesh* M_, size_t begin, size_t end) : M(M_), begin_idx(begin), end_idx(end) {}
  Iterator<Mesh, D> begin(){return Iterator<Mesh ,D>(M, begin_idx);}
  Iterator<Mesh, D> end(){return Iterator<Mesh, D>(M, end_idx);}

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

    
    Iterator_range<mesh<G, D>, D> cells(){return Iterator_range<mesh<G, D>, D>(this, 0, topo.nb_cells());}
    Iterator_range<mesh<G, D>, D-1> faces(){return Iterator_range<mesh<G, D>, D-1>(this, 0, topo.nb_faces());}
    Iterator_range<mesh<G, D>, 1> edges(){return Iterator_range<mesh<G, D>, 1>(this, 0, topo.nb_edges());}
    Iterator_range<mesh<G, D>, 0> vertices(){return Iterator_range<mesh<G, D>, 0>(this, 0, topo.nb_vertices());}


};


#endif //MESH