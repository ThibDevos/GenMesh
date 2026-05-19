#ifndef MESH_ENTITY
#define MESH_ENTITY

#include <cassert>
#include <cmath>
#include <numeric>
#include <core/math.h>
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

    shapes::ShapeType shape_type(){return shapes::get_shape_from_dim_vertices(D, nb_vertices());}

    size_t nb_vertices()
    {
      return M->topo.template get_connectivities<D,0>(index_).size();
    }

    auto coordinates()
    {
      if constexpr (D==0)
      {
        std::array<double, Mesh::dim_geo> coords;
        for(int i=0; i<Mesh::dim_geo; ++i){coords[i] = M->geo.coords[index_][i];}
        return coords;
      }
      else
      {
        std::vector<std::array<double, Mesh::dim_geo>> coords(nb_vertices());
        int j=0;
        for(auto && vj : this->vertices())
        {
          for(int i=0; i<Mesh::dim_geo; ++i){coords[j][i] = M->geo.coords[vj.index()][i];}
          ++j;
        }
        return coords;
      }
    }

    std::array<double, Mesh::dim_geo> barycenter()
    {
      std::array<double, Mesh::dim_geo> bary;
      double inv_dim = 1./nb_vertices();
      for(auto && vj : this->vertices())
      {
        for(int i=0; i<Mesh::dim_geo; ++i){bary[i] += M->geo.coords[vj.index()][i]*inv_dim;}
      }
      return bary;
    }

    double diameter()
    {
      if constexpr (D==0) return 0.;
      auto coord = this->coordinates();
      int nb_vert = nb_vertices();
      double d = 0;
      double dist = 0;
      for(int i=0; i<nb_vert; ++i)
      {
        for(int j=i+1; j<nb_vert; ++j)
        {
          for(int k=0; k<Mesh::dim_geo; ++k){dist+=std::pow(coord[i][k]-coord[j][k], 2);}
          d=std::max(d,dist);
        }
      }
      return std::sqrt(d);
    }

    double measure()
    {
      if constexpr (D==0) return 0.
      if constexpr (D==1) return this->diameter();
      shapes::Shape_type shape = this->shape_type();
      if constexpr (D==2)
      {
        
      }
      switch (shape)
      {
      case shapes::ShapeType::triangle:
        
        break;
      
      default:
        break;
      }
    }
  private:
    Mesh* M;
    size_t index_;
  
};


#endif //ENTITY