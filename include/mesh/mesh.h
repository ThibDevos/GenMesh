#ifndef MESH
#define MESH


#include <mesh/topology/topology.h>
#include <mesh/geometry/geometry.h>
#include <file_functions.h>
#include <mesh/entity_view.h>

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
    mesh(geometry<G> geo, topology<D> topo) : geo_(geo), topo_(topo) {;}

    static constexpr int dim_topo = D;
    static constexpr int dim_geo  = G;

    geometry<G> & geo() {return geo_;}
    const geometry<G> & geo() const {return geo_;}
    topology<D> & topo() {return topo_;}
    const topology<D> & topo() const {return topo_;}

    //iterator on Entity<Mesh,d,G> which allows to loop on the different entities of dim d

    template<size_t d> 
    mesh_range<mesh<G,D>, d> entities()
    {
      static_assert(d<=D);
      if (topo_.template get_connectivities<d,0>().size()==0) topo_.template build_connectivities<d,0>();
      return mesh_range<mesh<G, D>, d>(this, 0, topo_.nb_entities(d));
    }

    template<size_t d>
    mesh_range<const mesh<G,D>, d> entities() const
    {
      static_assert(d<=D);
      return mesh_range<const mesh<G, D>, d>(this, 0, topo_.nb_entities(d));
    }

    auto cells()     { return entities<D>();   }
    auto cells() const { return entities<D>(); }
    auto facets()    { return entities<D-1>(); }
    auto facets() const { return entities<D-1>(); }
    auto edges()     { return entities<1>();   }
    auto edges() const { return entities<1>(); }
    auto vertices()  { return entities<0>();   }
    auto vertices() const { return entities<0>(); }


    private:
      geometry<G> geo_;
      topology<D> topo_;

};


#endif //MESH