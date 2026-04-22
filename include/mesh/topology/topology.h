#ifndef MESH_TOPOLOGY_TOPOLOGY
#define MESH_TOPOLOGY_TOPOLOGY

#include <array>
#include <algorithm>
#include <cassert>
#include <vector>
#include <set>
#include <unordered_set>

#include <mesh/topology/shape.h>
#include <core/log.h>

/*!
\brief The topology of the mesh is set of the connectivities between the different shapes
*/
template<int D>
class topology
{
  using  relation_table = std::vector<std::vector<size_t>>;
  
  
  public:
  void set_nb_vertices(size_t n){nb_entities_[0] = n;}
  void set_nb_edges(size_t n){nb_entities_[1] = n;}
  void set_nb_facets(size_t n){nb_entities_[D-1] = n;}
  void set_nb_cells(size_t n){nb_entities_[D] = n;}
  
  size_t nb_vertices(){return nb_entities_[0];}
  size_t nb_edges(){return nb_entities_[1];}
  size_t nb_facets(){return nb_entities_[D-1];}
  size_t nb_cells(){return nb_entities_[D];}
  size_t nb_entities(size_t i){/*assert(i<D+1);*/ return nb_entities_[i];}
  
  std::array<size_t, D+1> nb_entities_{0};
  
  //shape types
  // shape_type[i] gives the type of the shape i. This shape is a cell (dim D).
  std::vector<ShapeType> shape_type;
  
  
  relation_table connectivities[D+1][D+1]; 
    
    template<size_t D1, size_t D2>
    std::vector<size_t>& get_connectivities(size_t index)
    {
      if(connectivities[D1][D2].size()==0)
      {
        if(D1==0) connectivities[D1][D2].resize(nb_entities_[D1]);
        build_connectivities<D1,D2>();
      }
      return connectivities[D1][D2][index];
    }

    /*!
  Build connectivities C[D1][D2]
  */
  template<size_t D1, size_t D2>
  void build_connectivities()
  {
    if constexpr (D2==0) //relation d-0 are built from cell-vertex relations and vertex-entity relations defined in shape.h
    {
      if(D1==1){build_edges();}
      if(D1==2){build_faces();}
      return;
    }
    if(connectivities[D2][D1].size()!=0) //if C[D2][D1] is already built, we can build C[D1][D2] by inverting the relation table C[D2][D1]
    {
      for(int i = 0; i<connectivities[D2][D1].size(); ++i)
      {
        for(int j=0; j<connectivities[D2][D1][i].size(); ++j)
        {
          size_t global_j = connectivities[D2][D1][i][j];
          connectivities[D1][D2][global_j].push_back(i);
        }
      }
    }
    else // C[D1][D2] = C[D1][k] \circ C[k][D2]. We use k=0
    {

      if(connectivities[0][D2].size()==0 && connectivities[D2][0].size()!=0)
      {
        connectivities[0][D2].resize(nb_entities_[0]);
        build_connectivities<0,D2>();
      }
      if(connectivities[D1][0].size()==0 && connectivities[0][D1].size()!=0)
      {
        connectivities[D1][0].resize(nb_entities_[D1]);
        build_connectivities<D1,0>();
      }
      for(int i=0; i<connectivities[D1][0].size(); ++i) //i is the index of the entity of dim D1
      {
        std::set<size_t> set;
        for(int j=0; j<connectivities[D1][0][i].size(); ++j)
        {
          size_t j_global = connectivities[D1][0][i][j]; //j_global is the global index of the jth vertex of entity i
          for(int k=0; k<connectivities[0][D2][j_global].size(); ++k)
          {
            size_t k_global = connectivities[0][D2][j_global][k];
            set.insert(k_global);
            }
          }
          connectivities[D1][D2][i].assign(set.begin(), set.end());
      }
    }
  }

  void build_edges()
  {
    std::cout<<"build edges"<<std::endl;
    std::vector<std::vector<size_t>>& edge_v = connectivities[1][0];
    edge_v.reserve(3*nb_entities_[D]);
    for(int i=0; i<nb_entities_[D]; ++i)
    {
      std::vector<size_t>& cell_v = connectivities[D][0][i]; //contains indices of vertices composing the cell i
      auto ed = shapes::edge_desc[static_cast<int>(shape_type[i])];
      for(int j = 0; j<ed.nb_edge; ++j)
      {
        size_t v0 = cell_v[ed.edge_vertex[j][0]];
        size_t v1 = cell_v[ed.edge_vertex[j][1]];
        if(v0>v1) std::swap(v0, v1);
        edge_v.push_back({v0,v1});

      }
    }
    std::sort(edge_v.begin(), edge_v.end());
    edge_v.erase(std::unique(edge_v.begin(), edge_v.end()), edge_v.end());
    nb_entities_[1] = edge_v.size();
  }

  void build_faces()
  {
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
          inverse[n-1-j] = rotation[j];
        }
       
        auto min = rotation < inverse ? rotation : inverse;
        if (min < can_norm)
        {
          can_norm = min;
        }
      }
      return can_norm;
    };

    std::vector<std::vector<size_t>> & face_v = connectivities[2][0];
    face_v.reserve(3 * nb_entities_[D]); // coarse estimation
    for (int i = 0; i < nb_entities_[D]; ++i)
    {
      std::vector<size_t> &cell_v = connectivities[D][0][i]; // contains indices of vertices composing the cell i
      auto fd = shapes::face_desc[static_cast<int>(shape_type[i])];
      for (int j = 0; j < fd.nb_face; ++j)
      {
        int offset = fd.face_vertex_offset[j];
        int nb_vert_loc =  fd.face_vertex_offset[j + 1] - offset;
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