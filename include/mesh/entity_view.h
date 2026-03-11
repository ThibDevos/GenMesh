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
    entity_view(Mesh * M_, size_t i) : M(M_), index_(i) {std::cout<<"Entity has dim "<<D<<"\n";}

    size_t index(){return index_;}

    template<size_t d>
    entity_range<Mesh,d> entities()
    {
      if constexpr(D==d)
      {
        std::cout<<"Return adjacency  TODO\n";
        std::vector<int> temp;
        return entity_range<Mesh,d>(M, &temp);
      }
      else
      {
        std::vector<int> & local_vertices = M->topo.template get_incidence<D,d>(index_);
        return entity_range<Mesh,d>(M,&local_vertices);
      }
    }
    auto cells()     { return entities<Mesh::dim_topo>();   }
    auto facets()    { return entities<Mesh::dim_topo-1>(); }
    auto edges()     { return entities<1>();   }
    auto vertices()  { return entities<0>();   }
  private:
    Mesh* M;
    size_t index_;
  
};


#endif //ENTITY