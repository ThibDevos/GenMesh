#ifndef MESH_ENTITY
#define MESH_ENTITY

#include <numeric>
#include <cassert>
#include <mesh/mesh.h>
#include <mesh/topology/shape.h>



template<typename Mesh, size_t D>
class entity_view;

template<typename Mesh, size_t D>
struct entity_iterator
{
  using iterator_category = std::forward_iterator_tag;
  using difference_type   = std::ptrdiff_t;
  using value_type        = entity_view<Mesh,D> ;
  using pointer           = value_type*;  
  using reference         = value_type&;

  entity_iterator(Mesh * M_, int i, std::vector<int> const * idxs) : M(M_), idx(i), indices(idxs) {}

  value_type operator*() const {return entity_view<Mesh,D>(M, (*indices)[idx]);}
  pointer operator->() const {return entity_view<Mesh,D>(M, (*indices)[idx]);}

  // Prefix increment
  entity_iterator &operator++()
  {
    idx++;
    return *this;
  }

  // Postfix increment
  entity_iterator operator++(int)
  {
    entity_iterator tmp = *this;
    ++(*this);
    return tmp;
  }
  //XXX is it true ?
  friend bool operator!=(const entity_iterator &a, const entity_iterator &b) { return a.idx != b.idx; };
  friend bool operator==(const entity_iterator &a, const entity_iterator &b) { return a.idx == b.idx; };

  private:
    Mesh* M;
    size_t idx;
    std::vector<int> const * indices;
};

template<typename Mesh, size_t D>
struct entity_range
{
  entity_range(Mesh * M_, std::vector<int> const * idxs) : M(M_), indices(idxs){}
  entity_iterator<Mesh,D> begin(){return entity_iterator<Mesh,D>(M,0, indices);}
  entity_iterator<Mesh,D> end(){return entity_iterator<Mesh,D>(M,indices->size(), indices);}
  private:
    Mesh* M;
    const std::vector<int> * indices;

};

/*!
\brief 
*/
template <typename Mesh, size_t D> 
class entity_view
{
  public:
    entity_view(Mesh * M_, size_t i) : M(M_), index_(i) {}

    size_t index(){return index_;}

    entity_range<Mesh,0> vertices()
    {
      std::vector<int> & local_vertices = M->topo.cell_vertex[index_];
      return entity_range<Mesh,0>(M,&local_vertices);
    }

  private:
    Mesh* M;
    size_t index_;
  
};


#endif //ENTITY