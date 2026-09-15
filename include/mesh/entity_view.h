#ifndef MESH_ENTITY
#define MESH_ENTITY

#include <cassert>
#include <cmath>
#include <core/math.h>
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

  entity_iterator(Mesh * M_, int i, std::span<const size_t> idxs) : M(M_), idx(i), indices(idxs) {}

  value_type operator*() const {return entity_view<Mesh,D>(M, indices[idx]);}
  pointer operator->() const {return entity_view<Mesh,D>(M, indices[idx]);}

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
    std::span<const size_t> indices;
};

template<typename Mesh, size_t D>
struct entity_range
{
  entity_range(Mesh * M_, std::span<const size_t> idxs) : M(M_), indices(idxs){}
  entity_iterator<Mesh,D> begin(){return entity_iterator<Mesh,D>(M,0, indices);}
  entity_iterator<Mesh,D> end(){return entity_iterator<Mesh,D>(M,indices.size(), indices);}
  private:
    Mesh* M;
    std::span<const size_t> indices;

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

    template<size_t d>
    entity_range<Mesh,d> entities()
    {
      if constexpr(D==d)
      {
        std::cout<<"Return adjacency  TODO\n";
        return entity_range<Mesh,d>(M, {});
      }
      else
      {
        auto local_indices = M->topo().template get_connectivities<D,d>(index_);
        return entity_range<Mesh,d>(M, local_indices);
      }
    }
    auto cells()     { return entities<Mesh::dim_topo>();   }
    auto facets()    { return entities<Mesh::dim_topo-1>(); }
    auto edges()     { return entities<1>();   }
    auto vertices()  { return entities<0>();   }

    shapes::ShapeType shape_type(){return shapes::get_shape_from_dim_vertices(D, nb_vertices());}

    size_t nb_vertices()
    {
      if constexpr(D==0) return 1;
      return M->topo().template get_connectivities<D,0>(index_).size();
    }

    auto coordinates()
    {
      std::vector<std::array<double, Mesh::dim_geo>> coords(nb_vertices());
      if constexpr (D == 0)
      {
        for(int i=0; i < Mesh::dim_geo; ++i)
        {
          coords[0][i] = M->geo().coords[this->index_][i];
        }
      }
      else
      {
        int j = 0;
        for (auto &&vj : this->vertices())
        {
          for (int i = 0; i < Mesh::dim_geo; ++i)
          {
            coords[j][i] = M->geo().coords[vj.index()][i];
          }
          ++j;
        }
      }
      return coords;
    }

    std::array<double, Mesh::dim_geo> barycenter()
    {
      std::array<double, Mesh::dim_geo> bary{0};
      if constexpr (D == 0)
      {
        for (int i = 0; i < Mesh::dim_geo; ++i)
        {
          bary[i] = M->geo().coords[this->index_][i];
        }
        return bary;
      }
      else
      {
        double inv_dim = 1. / nb_vertices();
        for (auto vj : this->vertices())
        {
          for (int i = 0; i < Mesh::dim_geo; ++i)
          {
            bary[i] += M->geo().coords[vj.index()][i] * inv_dim;
          }
        }
        return bary;
      }
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
          dist=0;
          for(int k=0; k<Mesh::dim_geo; ++k){dist+=std::pow(coord[i][k]-coord[j][k], 2);}
          d=std::max(d,dist);
        }
      }
      return std::sqrt(d);
    }

    /*!
    Gives the measure of the entity. This is computed geometrically. 
    It may be faster to use the mapping from a reference entity.
    */
    double measure()
    {
      if constexpr (D == 0)
        return 0.;
      if constexpr (D == 1)
        return this->diameter();
      shapes::ShapeType shape = this->shape_type();
      if constexpr (D == 2)
      {
        auto coord = this->coordinates();
        switch (shape)
        {
          case shapes::ShapeType::triangle:
          {
            double third_a = 0;
            double third_b = 0;
            if constexpr (Mesh::dim_geo == 3)
            {
              third_a = coord[0][2] - coord[1][2];
              third_b = coord[0][2] - coord[2][2];
            }
            std::array<double, 3> a{coord[0][0] - coord[1][0], coord[0][1] - coord[1][1], third_a};
            std::array<double, 3> b{coord[0][0] - coord[2][0], coord[0][1] - coord[2][1], third_b};
            double A = 0;
            for (int i = 0; i < 3; ++i)
            {
              A += std::pow(a[(i + 1) % 3] * b[(i + 2) % 3] - a[(i + 2) % 3] * b[(i + 1) % 3], 2);
            }
            return 0.5 * std::sqrt(A);
          }
          break;
          case shapes::ShapeType::quadrangle: // XXX the quadrangle must be convex
          {
            double third_a = 0;
            double third_b = 0;
            double third_c = 0;
            if constexpr (Mesh::dim_geo == 3)
            {
              third_a = coord[1][2] - coord[0][2];
              third_b = coord[1][2] - coord[2][2];
              third_c = coord[1][2] - coord[3][2];
            }
            std::array<double, 3> a{coord[1][0] - coord[0][0], coord[1][1] - coord[0][1], third_a};
            std::array<double, 3> b{coord[1][0] - coord[2][0], coord[1][1] - coord[2][1], third_b};
            std::array<double, 3> c{coord[1][0] - coord[3][0], coord[1][1] - coord[3][1], third_c};
            double A = 0;
            double B = 0;
            for (int i = 0; i < 3; ++i)
            {
              A += std::pow(a[(i + 1) % 3] * b[(i + 2) % 3] - a[(i + 2) % 3] * b[(i + 1) % 3], 2);
              B += std::pow(b[(i + 1) % 3] * c[(i + 2) % 3] - b[(i + 2) % 3] * c[(i + 1) % 3], 2);
            }
            return 0.5 * (std::sqrt(A) + std::sqrt(B));
          }
          default:
            bib::error("Shape not detected");
            break;
        }
      }
      if constexpr (D == 3)
      {

        auto coord = this->coordinates();
        // use the tetraedra decomposition
        auto volume_from_tet = [&coord](auto sub_tet)
        {
          double V = 0.;
          for (auto s : sub_tet)
          {
            std::array<double, 3> a{coord[s[0]][0] - coord[s[3]][0], coord[s[0]][1] - coord[s[3]][1], coord[s[0]][2] - coord[s[3]][2]};
            std::array<double, 3> b{coord[s[1]][0] - coord[s[3]][0], coord[s[1]][1] - coord[s[3]][1], coord[s[1]][2] - coord[s[3]][2]};
            std::array<double, 3> c{coord[s[2]][0] - coord[s[3]][0], coord[s[2]][1] - coord[s[3]][1], coord[s[2]][2] - coord[s[3]][2]};
            double V_t = 0;
            for (int i = 0; i < 3; ++i)
            {
              V_t += a[i] * (b[(i + 1) % 3] * c[(i + 2) % 3] - b[(i + 2) % 3] * c[(i + 1) % 3]);
            }
            V += std::fabs(V_t);
          }
          return V / 6.;
        };

        switch (shape)
        {
          case shapes::ShapeType::tetrahedron: 
          {
            std::array < std::array < size_t, 4 >, 1 > sub_tet = {{0, 1, 2, 3}};
            return volume_from_tet(sub_tet);
          }
          case shapes::ShapeType::hexahedron: 
          {
            std::array<std::array<size_t, 4>, 5> sub_tet{{{0, 1, 3, 4}, {1, 4, 5, 6}, {1, 3, 6, 7}, {3, 4, 6, 7}, {1, 3, 4, 6}}};
            return volume_from_tet(sub_tet);
          } 
          case shapes::ShapeType::prism: 
          {
            std::array<std::array<size_t, 4>, 3> sub_tet{{{0, 1, 2, 3}, {1, 2, 3, 4}, {2, 3, 4, 5}}};
            return volume_from_tet(sub_tet);
          } 
          case shapes::ShapeType::pyramid: 
          {
            std::array<std::array<size_t, 4>, 2> sub_tet{{{0, 1, 2, 4}, {0, 2, 3, 4}}};
            return volume_from_tet(sub_tet);
          } 
          default:
            bib::error("Shape not detected");
            break;
        }
      }
      return 0;
    }

  private:
    Mesh* M;
    size_t index_;
  
};


#endif //ENTITY