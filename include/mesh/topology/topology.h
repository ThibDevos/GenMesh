#ifndef MESH_TOPOLOGY_TOPOLOGY
#define MESH_TOPOLOGY_TOPOLOGY

#include <array>
#include <algorithm>
#include <cassert>
#include <map>
#include <vector>
#include <span>

#include <mesh/topology/shape.h>
#include <core/log.h>

/*!
\brief The topology of the mesh is set of the connectivities between the different shapes
*/
template <int D>
class topology
{

public:
  struct relation_table
  {
    std::vector<size_t> indices;
    std::vector<size_t> offsets;

    size_t size() const
    {
      return offsets.empty() ? 0 : offsets.size() - 1;
    }

    void resize(size_t count)
    {
      indices.clear();
      offsets.assign(count + 1, 0);
    }

    void assign(const std::vector<std::vector<size_t>> &relations)
    {
      resize(relations.size());
      for (size_t i = 0; i < relations.size(); ++i)
      {
        indices.insert(indices.end(), relations[i].begin(), relations[i].end());
        offsets[i + 1] = indices.size();
      }
    }

    void append(size_t row, size_t value)
    {
      indices.insert(indices.begin() + offsets[row + 1], value);
      for (size_t i = row + 1; i < offsets.size(); ++i)
      {
        ++offsets[i];
      }
    }

    void push_back(const std::vector<size_t> &relation)
    {
      if (offsets.empty())
      {
        offsets.push_back(0);
      }
      indices.insert(indices.end(), relation.begin(), relation.end());
      offsets.push_back(indices.size());
    }

    std::span<size_t> operator[](size_t i)
    {
      return std::span<size_t> (indices.data() + offsets[i], offsets[i+1] - offsets[i]);
    }

    std::span<const size_t> operator[](size_t i) const
    {
      return std::span<const size_t> (indices.data() + offsets[i], offsets[i+1] - offsets[i]);
    }

  };

private:
  mutable std::array<size_t, D + 1> nb_entities_{0};
  // shape types
  //  shape_type_[i] gives the type of the shape i. This shape is a cell (dim D).
  std::vector<shapes::ShapeType> shape_type_;
  mutable relation_table connectivities_[D + 1][D + 1];


public:
  void set_nb_vertices(size_t n) { nb_entities_[0] = n; }
  void set_nb_edges(size_t n) { nb_entities_[1] = n; }
  void set_nb_facets(size_t n) { nb_entities_[D - 1] = n; }
  void set_nb_cells(size_t n) { nb_entities_[D] = n; }

  size_t nb_vertices() const { return nb_entities_[0]; }
  size_t nb_edges() const { return nb_entities_[1]; }
  size_t nb_facets() const { return nb_entities_[D - 1]; }
  size_t nb_cells() const { return nb_entities_[D]; }
  size_t nb_entities(size_t i) const { /*assert(i<D+1);*/ return nb_entities_[i]; }

  std::vector<shapes::ShapeType>& shape_type(){return shape_type_;}
  std::vector<shapes::ShapeType> const& shape_type() const {return shape_type_;}


  template <size_t D1, size_t D2>
  std::span<size_t> get_connectivities(size_t index)
  {
    if (connectivities_[D1][D2].size() == 0)
    {
      build_connectivities<D1, D2>();
    }
    return connectivities_[D1][D2][index];
  }

  template <size_t D1, size_t D2>
  std::span<const size_t> get_connectivities(size_t index) const
  {
    return connectivities_[D1][D2][index];
  }
  
  template <size_t D1, size_t D2>
  relation_table &get_connectivities()
  {
    if (connectivities_[D1][D2].size() == 0)
    {
      build_connectivities<D1, D2>();
    }
    return connectivities_[D1][D2];
  }

  template <size_t D1, size_t D2>
  const relation_table &get_connectivities() const
  {
    if (connectivities_[D1][D2].size() == 0)
    {
      build_connectivities<D1, D2>();
    }
    return connectivities_[D1][D2];
  }

  /*!
  Build adjacency relation for entities of dim D1
  Two entities of dim d are adjacent if they share an entity of dim d-1
  */
  template<size_t D1>
  void build_adjacency() const
  {
    if constexpr (D1>0)
    {
      if (connectivities_[D1][D1 - 1].size() == 0)
      {
        build_connectivities<D1, D1 - 1>();
      }
      bib::debug_message("One construction done");
      if (connectivities_[D1 - 1][D1].size() == 0)
      {
        build_connectivities<D1 - 1, D1>();
      }
      bib::debug_message("Two constructions done");
      std::vector<std::vector<size_t>> adjacency_rows(connectivities_[D1][D1 - 1].size());
      auto &up_incidence = connectivities_[D1 - 1][D1];
      for (size_t i = 0; i < adjacency_rows.size(); ++i) // e is the set of indices of entities of dim D1-1 in the current entity
      {
        auto e = connectivities_[D1][D1 - 1][i];
        for (auto sub : e)
        {
          adjacency_rows[i].insert(adjacency_rows[i].end(), up_incidence[sub].begin(), up_incidence[sub].end());
        }
        auto &relation = adjacency_rows[i];
        std::sort(relation.begin(), relation.end());
        relation.erase(std::unique(relation.begin(), relation.end()), relation.end());
        relation.erase(std::remove(relation.begin(), relation.end(), i), relation.end());
      }
      connectivities_[D1][D1].assign(adjacency_rows);
    }
    else
    {
      auto &down_incidence = get_connectivities<D1+1,D1>();
      get_connectivities<D1,D1+1>();
      std::vector<std::vector<size_t>> adjacency_rows(connectivities_[D1][D1 + 1].size());
      for (size_t i = 0; i < adjacency_rows.size(); ++i) // e is the set of indices of entities of dim D1+1 in the current entity
      {
        auto e = connectivities_[D1][D1 + 1][i];
        for (auto sub : e)
        {
          adjacency_rows[i].insert(adjacency_rows[i].end(), down_incidence[sub].begin(), down_incidence[sub].end());
        }
        auto &relation = adjacency_rows[i];
        std::sort(relation.begin(), relation.end());
        relation.erase(std::unique(relation.begin(), relation.end()), relation.end());
        relation.erase(std::remove(relation.begin(), relation.end(), i), relation.end());
      }
      connectivities_[D1][D1].assign(adjacency_rows);
    }
  }
  /*!
  Build connectivities_ C[D1][D2]
  */
  template <size_t D1, size_t D2>
  void build_connectivities() const
  {
    bib::debug_message("start building %d %d", D1, D2);
    if constexpr (D1==D2){build_adjacency<D1>(); return;}
    auto inverse_relation = [this](int d1, int d2)
    {
      if (this->nb_entities_[d1] != 0)
      {
        connectivities_[d1][d2].resize(nb_entities_[d1]);
      }
      for (int i = 0; i < connectivities_[d2][d1].size(); ++i)
      {
        for (size_t global_j : connectivities_[d2][d1][i])
        {
          connectivities_[d1][d2].append(global_j, i);
        }
      }
    };

    
    if (connectivities_[D2][D1].size() != 0) // if C[D2][D1] is already built, we can build C[D1][D2] by inverting the relation table C[D2][D1]
    {
      bib::debug_message("In case we can inverse");
      inverse_relation(D1,D2);
      return;
    }
    else // we use the relation C[D1][0] and C[0][D2]
    {
      bib::debug_message("In case we can't inverse");
      if constexpr (D2 == 0) // relation d-0 are built from cell-vertex relations and vertex-entity relations defined in shape.h
      {
        bib::debug_message("case D2==0");
        if constexpr (D1 == 1)
        {
          build_edges();
        }
        if constexpr (D1 == 2 && D > 2)
        {
          build_faces();
        }
        return;
      }
      if constexpr (D1 == 0)
      {
        bib::debug_message("case D1==0");
        build_connectivities<D2, D1>();
        inverse_relation(D1, D2);
        return;
      }
      if constexpr (D1<D2)
      {
        build_connectivities<D2,D1>();
        inverse_relation(D1,D2);
        return;
      }
      if (connectivities_[D2][0].size() == 0 )
      {
        build_connectivities<D2, 0>();
      }
      if (connectivities_[D1][0].size() == 0)
      {
        build_connectivities<D1, 0>();
      }
      bib::debug_message("here");
      connectivities_[D1][D2].resize(nb_entities_[D1]);

      // assuming we have C[D2][0]
      std::map<std::vector<size_t>, size_t> C_D2_0;
      for (int i = 0; i < nb_entities_[D2]; ++i)
      {
        auto vertices = connectivities_[D2][0][i];
        C_D2_0[std::vector<size_t>(vertices.begin(), vertices.end())] = i;
      }
      for (int i = 0; i < nb_entities_[D1]; ++i)
      {
        if (D2 == 1)
        {
          auto cell_v = connectivities_[D1][0][i]; // contains indices of vertices composing the cell i
          auto ed = shapes::edge_desc[static_cast<int>(shapes::get_shape_from_dim_vertices(D1, cell_v.size()))];
          for (int j = 0; j < ed.nb_edge; ++j)
          {
            size_t v0 = cell_v[ed.edge_vertex[j][0]];
            size_t v1 = cell_v[ed.edge_vertex[j][1]];
            if (v0 > v1)
              std::swap(v0, v1);
            auto found = C_D2_0.find({v0, v1});
            if (found != C_D2_0.end())
              connectivities_[D1][D2].append(i, found->second);
          }
        }
        if (D2 == 2) // if D2==2, D1==3 by definition
        {
          auto normalize_indices = [](std::vector<size_t> &indices)
          {
            int n = indices.size();
            std::vector<size_t> can_norm(indices);
            std::vector<size_t> rotation(n);
            std::vector<size_t> inverse(indices.rbegin(), indices.rend());
            if (inverse < can_norm)
            {
              can_norm = inverse;
            }
            for (int i = 1; i < n; ++i) // each iteration is a rotation in both sens
            {
              for (int j = 0; j < n; ++j)
              {
                rotation[j] = indices[(j + i) % n];
                inverse[n - 1 - j] = rotation[j];
              }

              auto min = rotation < inverse ? rotation : inverse;
              if (min < can_norm)
              {
                can_norm = min;
              }
            }
            return can_norm;
          };

          auto cell_v = connectivities_[D1][0][i]; // contains indices of vertices composing the cell i
          auto fd = shapes::face_desc[static_cast<int>(shape_type_[i])];
          for (int j = 0; j < fd.nb_face; ++j)
          {
            int offset = fd.face_vertex_offset[j];
            int nb_vert_loc = fd.face_vertex_offset[j + 1] - offset;
            std::vector<size_t> vertices(nb_vert_loc);
            for (int k = 0; k < nb_vert_loc; ++k)
            {
              vertices[k] = cell_v[fd.face_vertex[offset + k]];
            }
            vertices = normalize_indices(vertices);
            auto found = C_D2_0.find(vertices);
            if (found != C_D2_0.end())
              connectivities_[D1][D2].append(i, found->second);
          }
        }
      }

      bib::debug_message("finished building");
    }

  }

  void build_edges() const
  {
    std::cout << "build edges" << std::endl;
    std::vector<std::vector<size_t>> edge_v;
    edge_v.reserve(3 * nb_entities_[D]);
    for (int i = 0; i < nb_entities_[D]; ++i)
    {
      auto cell_v = connectivities_[D][0][i]; // contains indices of vertices composing the cell i
      auto ed = shapes::edge_desc[static_cast<int>(shape_type_[i])];
      for (int j = 0; j < ed.nb_edge; ++j)
      {
        size_t v0 = cell_v[ed.edge_vertex[j][0]];
        size_t v1 = cell_v[ed.edge_vertex[j][1]];
        if (v0 > v1)
          std::swap(v0, v1);
        edge_v.push_back({v0, v1});
      }
    }
    std::sort(edge_v.begin(), edge_v.end());
    edge_v.erase(std::unique(edge_v.begin(), edge_v.end()), edge_v.end());
    connectivities_[1][0].assign(edge_v);
    nb_entities_[1] = edge_v.size();
  }

  void build_faces() const
  {
    bib::debug_message("build faces");
    assert(D > 2);

    auto normalize_indices = [](std::vector<size_t> indices)
    {
      int n = indices.size();
      std::vector<size_t> can_norm(indices);
      std::vector<size_t> rotation(n);
      std::vector<size_t> inverse(indices.rbegin(), indices.rend());
      if (inverse < can_norm)
      {
        can_norm = inverse;
      }
      for (int i = 1; i < n; ++i) // each iteration is a rotation in both sens
      {
        for (int j = 0; j < n; ++j)
        {
          rotation[j] = indices[(j + i) % n];
          inverse[n - 1 - j] = rotation[j];
        }

        auto min = rotation < inverse ? rotation : inverse;
        if (min < can_norm)
        {
          can_norm = min;
        }
      }
      return can_norm;
    };

    std::vector<std::vector<size_t>> face_v;
    face_v.reserve(3 * nb_entities_[D]); // coarse estimation
    for (int i = 0; i < nb_entities_[D]; ++i)
    {
      auto cell_v = connectivities_[D][0][i]; // contains indices of vertices composing the cell i
      auto fd = shapes::face_desc[static_cast<int>(shape_type_[i])];
      for (int j = 0; j < fd.nb_face; ++j)
      {
        int offset = fd.face_vertex_offset[j];
        int nb_vert_loc = fd.face_vertex_offset[j + 1] - offset;
        std::vector<size_t> vertices(nb_vert_loc);
        for (int k = 0; k < nb_vert_loc; ++k)
        {
          vertices[k] = cell_v[fd.face_vertex[offset + k]];
        }
        face_v.push_back(normalize_indices(vertices));
      }
    }
    std::sort(face_v.begin(), face_v.end());
    face_v.erase(std::unique(face_v.begin(), face_v.end()), face_v.end());
    connectivities_[2][0].assign(face_v);
    nb_entities_[2] = face_v.size();
  }
};

#endif