#ifndef PARTITION_H
#define PARTITION_H

#include "core/log.h"
#include "mesh/geometry/geometry.h"
#include "mesh/topology/topology.h"
#include <mesh/mesh.h>
#include <mpi.h>
#include <numeric>
#include <parallel/wrapper_mpi.h>
#include <algorithm>
#include <array>
#include <limits>
#include <stdexcept>
#include <unordered_map>

/*
Creates a partition using Morton code
*/
template <size_t G>
class Morton_partition
{

  enum TAGS {SI, COORD, SHAPE, CVI, CVO, GCI, GVI, NOC, NOV};

  template<size_t D>
  struct local_mesh_data
  {
    //geometry
    std::vector<std::array<double,G>> coords;

    //topology
    std::vector<shapes::ShapeType> shape_type;
    topology<D>::relation_table cell_vertex;
    std::vector<size_t> global_cell_id;
    std::vector<size_t> global_vertex_id;
    size_t nb_owned_cells = 0;
    size_t nb_owned_vertices = 0;
  };

  MPI_Datatype MPI_coord_type;

  public:
  template <size_t D>
  mesh<G,D> partition(mesh<G, D> const &M)
  {
    MPI_Type_contiguous(G, MPI_DOUBLE, &MPI_coord_type); //creates a MPI_Datatype that is composed of G contiguous double.
                                                                                //Used for sending the coordinates
    MPI_Type_commit(&MPI_coord_type);//commits the new type to all proc

    local_mesh_data<D> lmdr;
    if(parallel::is_root())
    {
      int nb_cells = M.topo().nb_cells();
      std::vector<std::array<double, D>> centroid_coords(nb_cells);
      for(auto && c : M.cells())
      {
        centroid_coords[c.index()] = c.barycenter();
      }

      std::vector<std::array<uint32_t, D>> int_coords(nb_cells);
      std::vector<uint64_t> Morton_code(nb_cells);
      std::vector<int> indices(nb_cells); // Morton_code will be sorted. indices helps keeping a relation of indices between Morton_code and orignal indices
      std::iota(indices.begin(), indices.end(), 0);
      
      // transform the floating points coordiantes of the vertices to Morton code
      float_coord_to_int(centroid_coords, int_coords);
      int_coord_to_Morton(Morton_code, int_coords);
      radix_sort_indices(Morton_code, indices);
      
      std::vector<size_t> owners(nb_cells);
      for(int i=0; i<nb_cells; i++)
      {
        owners[indices[i]] = static_cast<int>((i * parallel::size()) / nb_cells) ;
      }
      //XXX ghosts

      //build parallele::size() local_mesh
      local_mesh_data<D> local_meshes_data[parallel::size()];
      for(int r=0; r<parallel::size(); ++r)
      {
        build_local_mesh_data<D>(M, owners, r, local_meshes_data[r]);
      }

      lmdr = std::move(local_meshes_data[0]);
      send_local_mesh_data(local_meshes_data);
    }
    if(!parallel::is_root())
      lmdr = recv_local_mesh_data<D>();
    mesh<G,D> M_loc = build_mesh_from_local_data(lmdr);
    return M_loc;
  }

  template<size_t D>
  void build_local_mesh_data(mesh<G,D> const & M, std::vector<size_t>& owners, int r, local_mesh_data<D>& local_mesh_data)
  {
    bib::debug_message("In build_local_mesh_data");
    std::unordered_map<size_t, size_t> global_to_local_vertices;
    auto & cell_vertex = M.topo().template get_connectivities<D,0>();
    size_t current_vertex = 0;
    
    std::vector<size_t> indices;
    for(int i=0; i<owners.size(); ++i)
    {
      if(owners[i]==r) //the cell is added
      {
        indices.clear();
        local_mesh_data.global_cell_id.push_back(i);
        local_mesh_data.shape_type.push_back(M.topo().shape_type()[i]);
        auto global_vertices = cell_vertex[i];
        for(auto id_v : global_vertices)
        {
          if(global_to_local_vertices.find(id_v)==global_to_local_vertices.end())
          {
            local_mesh_data.global_vertex_id.push_back(id_v);
            global_to_local_vertices[id_v] = current_vertex;
            local_mesh_data.coords.push_back(M.geo().coords[id_v]);
            ++current_vertex;
          }
          indices.push_back(global_to_local_vertices[id_v]);
        }
        local_mesh_data.cell_vertex.push_back(indices);
      }
    }
    local_mesh_data.nb_owned_cells = local_mesh_data.cell_vertex.size();
    local_mesh_data.nb_owned_vertices = local_mesh_data.coords.size();
  }

  template<size_t D>
  mesh<G,D> build_mesh_from_local_data(local_mesh_data<D> & local_mesh_data)
  {
    bib::debug_message("In build_mesh_from_local_data");
    geometry<G> loc_geo;
    topology<D> loc_topo;
    
    loc_geo.coords = local_mesh_data.coords;

    loc_topo.shape_type() = local_mesh_data.shape_type;
    loc_topo.template get_connectivities<D,0>() = local_mesh_data.cell_vertex;
    loc_topo.set_nb_cells(local_mesh_data.nb_owned_cells);

    mesh<G,D> M(loc_geo, loc_topo);
    return M;

  }
  
  template<size_t D>
  void send_local_mesh_data(local_mesh_data<D> lmd[])
  {
    bib::debug_message("In send");
    if(parallel::is_root())
    {
      for(int r=1; r<parallel::size(); ++r)
      {
        auto & lmdr = lmd[r];
        
        size_t size_cells = lmdr.nb_owned_cells;
        size_t size_vertices = lmdr.nb_owned_vertices;
        size_t size_indices = lmdr.cell_vertex.indices.size();

        MPI_Send(&size_indices, 1, MPI_UNSIGNED_LONG, r, TAGS::SI, parallel::comm());
        bib::debug_message("Sent size_indices");
        MPI_Send(&lmdr.nb_owned_cells, 1, MPI_UNSIGNED_LONG, r, TAGS::NOC, parallel::comm());
        bib::debug_message("Sent nb_owned_cells");
        MPI_Send(&lmdr.nb_owned_vertices, 1, MPI_UNSIGNED_LONG, r, TAGS::NOV, parallel::comm());
        bib::debug_message("Sent nb_owned_vertices");
        MPI_Send(lmdr.coords.data(), size_vertices, MPI_coord_type, r, TAGS::COORD, parallel::comm());
        bib::debug_message("Sent coords");
        MPI_Send(lmdr.shape_type.data(), size_cells, MPI_INT, r, TAGS::SHAPE, parallel::comm());
        bib::debug_message("Sent shape_type");
        MPI_Send(lmdr.cell_vertex.indices.data(), size_indices, MPI_UNSIGNED_LONG, r, TAGS::CVI, parallel::comm());
        bib::debug_message("Sent indices");
        MPI_Send(lmdr.cell_vertex.offsets.data(), size_cells + 1, MPI_UNSIGNED_LONG, r, TAGS::CVO, parallel::comm());
        bib::debug_message("Sent offsets");
        MPI_Send(lmdr.global_cell_id.data(), size_cells, MPI_UNSIGNED_LONG, r, TAGS::GCI, parallel::comm());
        bib::debug_message("Sent globale_cell_id");
        MPI_Send(lmdr.global_vertex_id.data(), size_vertices, MPI_UNSIGNED_LONG, r, TAGS::GVI, parallel::comm());
        bib::debug_message("Sent globale_vertex_id");
      }
    }
  }

  template<size_t D>
  local_mesh_data<D> recv_local_mesh_data()
  {
    bib::debug_message("In recv");
    local_mesh_data<D> lmdr;
    size_t size_indices;
    
    MPI_Recv(&size_indices, 1, MPI_UNSIGNED_LONG, 0, TAGS::SI, parallel::comm(), MPI_STATUS_IGNORE);
    MPI_Recv(&lmdr.nb_owned_vertices, 1, MPI_UNSIGNED_LONG, 0, TAGS::NOV, parallel::comm(), MPI_STATUS_IGNORE);
    MPI_Recv(&lmdr.nb_owned_cells, 1, MPI_UNSIGNED_LONG, 0, TAGS::NOC, parallel::comm(), MPI_STATUS_IGNORE);

    size_t size_cells = lmdr.nb_owned_cells;
    size_t size_vertices = lmdr.nb_owned_vertices;
    lmdr.coords.resize(size_vertices);
    lmdr.shape_type.resize(size_cells);
    lmdr.cell_vertex.indices.resize(size_indices);
    lmdr.cell_vertex.offsets.resize(size_cells+1);
    lmdr.global_cell_id.resize(size_cells);
    lmdr.global_vertex_id.resize(size_vertices);

    MPI_Recv(lmdr.coords.data(), size_vertices, MPI_coord_type, 0, TAGS::COORD, parallel::comm(), MPI_STATUS_IGNORE);
    MPI_Recv(lmdr.shape_type.data(), size_cells, MPI_INT, 0, TAGS::SHAPE, parallel::comm(), MPI_STATUS_IGNORE);
    MPI_Recv(lmdr.cell_vertex.indices.data(), size_indices, MPI_UNSIGNED_LONG, 0, TAGS::CVI, parallel::comm(), MPI_STATUS_IGNORE);
    MPI_Recv(lmdr.cell_vertex.offsets.data(), size_cells + 1, MPI_UNSIGNED_LONG, 0, TAGS::CVO, parallel::comm(), MPI_STATUS_IGNORE);
    MPI_Recv(lmdr.global_cell_id.data(), size_cells, MPI_UNSIGNED_LONG, 0, TAGS::GCI, parallel::comm(), MPI_STATUS_IGNORE);
    MPI_Recv(lmdr.global_vertex_id.data(), size_vertices, MPI_UNSIGNED_LONG, 0, TAGS::GVI, parallel::comm(), MPI_STATUS_IGNORE);
    return lmdr;
  }

private:
  void float_coord_to_int(std::vector<std::array<double, G>> &coords, std::vector<std::array<uint32_t, G>> &int_coords)
  {
    constexpr uint64_t max_coord = ((uint64_t(1) << L) - 1);
    double min[G];
    double max[G];
    for (int i = 0; i < G; ++i)
    {
      min[i] = std::numeric_limits<double>::max();
      max[i] = std::numeric_limits<double>::lowest();
    }
    for (auto const &c : coords)
    {
      for (int d = 0; d < G; ++d)
      {
        double pos = c[d];
        if (pos < min[d])
          min[d] = pos;
        if (pos > max[d])
          max[d] = pos;
      }
    }
    // Max length
    double extent[G];
    for (int d = 0; d < G; ++d)
    {
      extent[d] = max[d] - min[d];
    }

    double Max = std::max(extent[0], extent[1]);
    if constexpr (G == 3)
      Max = std::max(Max, extent[2]);
    if (Max == 0.0)
      Max = 1.0;

    {
      int i = 0;
      for (auto const &c : coords)
      {
        for (int d = 0; d < G; ++d)
        {
          int_coords[i][d] = static_cast<uint32_t>(std::clamp((c[d] - min[d]) / Max, 0., 1.) * max_coord);
        }
        ++i;
      }
    }
  }

  void int_coord_to_Morton(std::vector<uint64_t> & Morton_code, std::vector<std::array<uint32_t, G>> &int_coords)
  {
    int i = 0;
    for (auto &c : int_coords)
    {
      Morton_code[i] = encodeMorton(c);
      ++i;
    }
  }

  void radix_sort_indices(std::vector<uint64_t> &codes, std::vector<int> &indices)
  {
    size_t n = codes.size();
    if (n == 0)
      return;

    std::vector<uint64_t> temp_codes(n);
    std::vector<int> temp_indices(n);

    for (int shift = 0; shift < 64; shift += 8)
    {
      size_t count[257] = {0};

      for (size_t i = 0; i < n; i++)
      {
        size_t bucket = (codes[i] >> shift) & 0xFF;
        count[bucket + 1]++;
      }

      for (size_t i = 0; i < 256; i++)
      {
        count[i + 1] += count[i];
      }

      for (size_t i = 0; i < n; i++)
      {
        size_t bucket = (codes[i] >> shift) & 0xFF;
        size_t pos = count[bucket]++;
        temp_codes[pos] = codes[i];
        temp_indices[pos] = indices[i];
      }

      codes.swap(temp_codes);
      indices.swap(temp_indices);
    }
  }
  // from integer coordinates to Morton code
  template <typename T> // T is a container of D unint32_t
  uint64_t encodeMorton(T x)
  {
    uint64_t a = 0;
    for (int d = 0; d < G; ++d)
    {
      a |= expand_bits(x[d]) << d;
    }
    return a;
  }

  // Spread the bits of a coordinate so that they can be interleaved with the
  // other coordinates. The 2D and 3D variants use different masks.
  uint64_t expand_bits(uint32_t x)
  {
    if constexpr (G == 2)
    {
      uint64_t y = x;
      y = (y | (y << 16)) & 0x0000FFFF0000FFFFULL;
      y = (y | (y << 8)) & 0x00FF00FF00FF00FFULL;
      y = (y | (y << 4)) & 0x0F0F0F0F0F0F0F0FULL;
      y = (y | (y << 2)) & 0x3333333333333333ULL;
      y = (y | (y << 1)) & 0x5555555555555555ULL;
      return y;
    }
    else if constexpr (G == 3)
    {
      uint64_t y = x & 0x1fffff;
      y = (y | y << 32) & 0x1f00000000ffff;
      y = (y | y << 16) & 0x1f0000ff0000ff;
      y = (y | y << 8) & 0x100f00f00f00f00f;
      y = (y | y << 4) & 0x10c30c30c30c30c3;
      y = (y | y << 2) & 0x1249249249249249;
      return y;
    }
    else
    {
      throw std::runtime_error("Morton encoding is only supported for 2D or 3D");
    }
  }

  static constexpr uint32_t L = []
  {
    if constexpr (G == 2)
      return 32;
    else if constexpr (G == 3)
      return 21;
    else
      std::runtime_error("Wrong dimension");
  }(); // each coordinate is represented by 32 bits in 2D (2*32=64 bits) or 21 bits in 3D (3*21 = 63)
};

#endif