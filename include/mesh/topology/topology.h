#ifndef MESH_TOPOLOGY_TOPOLOGY
#define MESH_TOPOLOGY_TOPOLOGY

#include <array>
#include <algorithm>
#include <cassert>
#include <map>
#include <vector>
#include <set>
#include <unordered_set>

#include <mesh/topology/shape.h>
#include <core/log.h>

/*!
\brief The topology of the mesh is set of the connectivities between the different shapes
*/
template <int D>
class topology
{
  using relation_table = std::vector<std::vector<size_t>>;

public:
  void set_nb_vertices(size_t n) { nb_entities_[0] = n; }
  void set_nb_edges(size_t n) { nb_entities_[1] = n; }
  void set_nb_facets(size_t n) { nb_entities_[D - 1] = n; }
  void set_nb_cells(size_t n) { nb_entities_[D] = n; }

  size_t nb_vertices() { return nb_entities_[0]; }
  size_t nb_edges() { return nb_entities_[1]; }
  size_t nb_facets() { return nb_entities_[D - 1]; }
  size_t nb_cells() { return nb_entities_[D]; }
  size_t nb_entities(size_t i) { /*assert(i<D+1);*/ return nb_entities_[i]; }

  std::array<size_t, D + 1> nb_entities_{0};

  // shape types
  //  shape_type[i] gives the type of the shape i. This shape is a cell (dim D).
  std::vector<shapes::ShapeType> shape_type;

  relation_table connectivities[D + 1][D + 1];

  template <size_t D1, size_t D2>
  std::vector<size_t> &get_connectivities(size_t index)
  {
    if (connectivities[D1][D2].size() == 0)
    {
      build_connectivities<D1, D2>();
    }
    return connectivities[D1][D2][index];
  }
  template <size_t D1, size_t D2>
  std::vector<std::vector<size_t>> &get_connectivities()
  {
    if (connectivities[D1][D2].size() == 0)
    {
      build_connectivities<D1, D2>();
    }
    return connectivities[D1][D2];
  }

  /*!
  Build adjacency relation for entities of dim D1
  Two entities of dim d are adjacent if they share an entity of dim d-1
  */
  template<size_t D1>
  void build_adjacency()
  {
    if constexpr (D1>0)
    {
      if (connectivities[D1][D1 - 1].size() == 0)
      {
        build_connectivities<D1, D1 - 1>();
      }
      bib::debug_message("One construction done");
      if (connectivities[D1 - 1][D1].size() == 0)
      {
        build_connectivities<D1 - 1, D1>();
      }
      bib::debug_message("Two constructions done");
      connectivities[D1][D1].resize(connectivities[D1][D1 - 1].size());

      auto &adjacency = connectivities[D1][D1];
      auto &up_incidence = connectivities[D1 - 1][D1];
      size_t i = 0;
      for (auto e : connectivities[D1][D1 - 1]) // e is the set of indices of entities of dim D1-1 in the current entity
      {
        for (auto sub : e)
        {
          adjacency[i].insert(adjacency[i].end(), up_incidence[sub].begin(), up_incidence[sub].end());
        }
        std::sort(adjacency[i].begin(), adjacency[i].end());
        adjacency[i].erase(std::unique(adjacency[i].begin(), adjacency[i].end()), adjacency[i].end());
        adjacency[i].erase(std::remove(adjacency[i].begin(), adjacency[i].end(), i), adjacency[i].end());
        ++i;
      }
    }
    else
    {
      auto &down_incidence = get_connectivities<D1+1,D1>();
      get_connectivities<D1,D1+1>();
      connectivities[D1][D1].resize(connectivities[D1][D1 + 1].size());
      auto &adjacency = connectivities[D1][D1];

      size_t i = 0;
      for (auto e : connectivities[D1][D1 + 1]) // e is the set of indices of entities of dim D1+1 in the current entity
      {
        for (auto sub : e)
        {
          adjacency[i].insert(adjacency[i].end(), down_incidence[sub].begin(), down_incidence[sub].end());
        }
        std::sort(adjacency[i].begin(), adjacency[i].end());
        adjacency[i].erase(std::unique(adjacency[i].begin(), adjacency[i].end()), adjacency[i].end());
        adjacency[i].erase(std::remove(adjacency[i].begin(), adjacency[i].end(), i), adjacency[i].end());
        ++i;
      }
    }
  }
  /*!
  Build connectivities C[D1][D2]
  */
  template <size_t D1, size_t D2>
  void build_connectivities()
  {
    bib::debug_message("start building %d %d", D1, D2);
    if constexpr (D1==D2){build_adjacency<D1>(); return;}
    auto inverse_relation = [this](int d1, int d2)
    {
      if (this->nb_entities_[d1] != 0)
      {
        connectivities[d1][d2].resize(nb_entities_[d1]);
      }
      for (int i = 0; i < connectivities[d2][d1].size(); ++i)
      {
        for (int j = 0; j < connectivities[d2][d1][i].size(); ++j)
        {
          size_t global_j = connectivities[d2][d1][i][j];
          connectivities[d1][d2][global_j].push_back(i);
        }
      }
    };

    
    if (connectivities[D2][D1].size() != 0) // if C[D2][D1] is already built, we can build C[D1][D2] by inverting the relation table C[D2][D1]
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
        if (D1 == 1)
        {
          build_edges();
        }
        if (D1 == 2)
        {
          build_faces();
        }
        return;
      }
      if constexpr (D1 == 0)
      {
        bib::debug_message("case D1==0");
        build_connectivities<D2, D1>();
        return;
      }
      if constexpr (D1<D2)
      {
        build_connectivities<D2,D1>();
        inverse_relation(D1,D2);
        return;
      }
      if (connectivities[D2][0].size() == 0 )
      {
        build_connectivities<D2, 0>();
      }
      if (connectivities[D1][0].size() == 0)
      {
        build_connectivities<D1, 0>();
      }
      bib::debug_message("here");
      connectivities[D1][D2].resize(nb_entities_[D1]);

      // assuming we have C[D2][0]
      std::map<std::vector<size_t>, size_t> C_D2_0;
      for (int i = 0; i < nb_entities_[D2]; ++i)
      {
        C_D2_0[connectivities[D2][0][i]] = i;
      }
      for (int i = 0; i < nb_entities_[D1]; ++i)
      {
        if (D2 == 1)
        {
          std::vector<size_t> &cell_v = connectivities[D1][0][i]; // contains indices of vertices composing the cell i
          auto ed = shapes::edge_desc[static_cast<int>(shapes::get_shape_from_dim_vertices(D1, cell_v.size()))];
          for (int j = 0; j < ed.nb_edge; ++j)
          {
            size_t v0 = cell_v[ed.edge_vertex[j][0]];
            size_t v1 = cell_v[ed.edge_vertex[j][1]];
            if (v0 > v1)
              std::swap(v0, v1);
            auto found = C_D2_0.find({v0, v1});
            if (found != C_D2_0.end())
              connectivities[D1][D2][i].push_back(found->second);
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

          std::vector<size_t> &cell_v = connectivities[D1][0][i]; // contains indices of vertices composing the cell i
          auto fd = shapes::face_desc[static_cast<int>(shape_type[i])];
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
              connectivities[D1][D2][i].push_back(found->second);
          }
        }
      }

      bib::debug_message("finished building");
    }

  }

  void build_edges()
  {
    std::cout << "build edges" << std::endl;
    std::vector<std::vector<size_t>> &edge_v = connectivities[1][0];
    edge_v.reserve(3 * nb_entities_[D]);
    for (int i = 0; i < nb_entities_[D]; ++i)
    {
      std::vector<size_t> &cell_v = connectivities[D][0][i]; // contains indices of vertices composing the cell i
      auto ed = shapes::edge_desc[static_cast<int>(shape_type[i])];
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
    nb_entities_[1] = edge_v.size();
  }

  void build_faces()
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

    std::vector<std::vector<size_t>> &face_v = connectivities[2][0];
    face_v.reserve(3 * nb_entities_[D]); // coarse estimation
    for (int i = 0; i < nb_entities_[D]; ++i)
    {
      std::vector<size_t> &cell_v = connectivities[D][0][i]; // contains indices of vertices composing the cell i
      auto fd = shapes::face_desc[static_cast<int>(shape_type[i])];
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
    nb_entities_[2] = face_v.size();
  }
};

#endif