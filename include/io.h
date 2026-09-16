#ifndef IO_H
#define IO_H

#include "parallel/wrapper_mpi.h"
#include <array>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <mesh/mesh.h>
#include <mesh/topology/shape.h>
#include <file_functions.h>

template<typename Mesh>
struct gmesh
{



 std::map<int, int> gtypes_dim{
      {15, 0}, {1, 1}, {2, 2}, {3, 2}, {4, 3}, {5, 3}, {6, 3}, {7, 3}};
  std::map<int, shapes::ShapeType> gtypes_types{
      {15, shapes::ShapeType::vertex}, {1, shapes::ShapeType::edge}, {2, shapes::ShapeType::triangle}, {3, shapes::ShapeType::quadrangle}, {4, shapes::ShapeType::tetrahedron}, {5, shapes::ShapeType::hexahedron}, {6, shapes::ShapeType::prism}, {7, shapes::ShapeType::pyramid}};


  
  template<typename Shape>
  static void build_element_from_vertices(Mesh & M, std::istringstream & iss)
  {
    int n_vertices = Shape::nb_sub_included[0];
    std::vector<size_t> local_vertices(n_vertices);
    for(int i=0; i<n_vertices; ++i)
    {
      iss>>local_vertices[i];
      --local_vertices[i];
    }
    M.topo().template get_connectivities<Shape::D,0>().push_back(local_vertices);
    
    // switch (Shape::D)
    // {
    // case M.dim_topo: //cell
    //   M.topo().connectivi
    //   break;
    // case M.dim_topo - 1: //facet 
    //    M.topo().facet_connectivities.lowest.push_back(local_vertices);
    //   break;
    // case 1: //edge
    //    M.topo().edge_connectivities.lowest.push_back(local_vertices);
    //   break;
    
    // default:
    //   break;
    // }
  }

  using builders = void(*)(Mesh & M, std::istringstream & iss);

  static inline constexpr std::array<builders,
  static_cast<int>(shapes::ShapeType::count)> f_builders=  { //should follow the order of enum ShapeType in shape.h
    &build_element_from_vertices<shapes::vertex>,
    &build_element_from_vertices<shapes::edge>,
    &build_element_from_vertices<shapes::triangle>,
    &build_element_from_vertices<shapes::tetrahedron>,
    &build_element_from_vertices<shapes::quadrangle>,
    &build_element_from_vertices<shapes::hexahedron>,
    &build_element_from_vertices<shapes::prism>,
    &build_element_from_vertices<shapes::pyramid>
  };

  

  void add_incidence(Mesh & M, std::istringstream & iss, int type)
  {
    int dim_el = gtypes_dim[type];
    switch (dim_el)
    {
    case M.dim_topo:
      add_cell_vertex(M, iss, type);
      break;
    case  M.dim_topo - 1:
      add_facet_vertex(M, iss, type);
      break;
    case  1:
      add_edge_vertex(M, iss, type);
      break;
    
    default:
      std::runtime_error("mesh::read : wrong topological dimension");
      break;
    }
  }

  void read_gmsh(Mesh & M, std::string meshfile)
  {
    if(!parallel::is_root()) return;
    std::istringstream iss;
    std::string line;
    std::ifstream f(meshfile);
    int trash = 0;

    if (!f)
    {
      throw std::runtime_error("mesh::read : file " + meshfile + " does not exist");
    }
    getline(f, line);
    while (line != "$Nodes")
    {
      getline(f, line);
    }
    getline(f, line); // Number of vertices
    iss.str(line);
    int nb_vertices;
    iss >> nb_vertices;
    M.topo().set_nb_vertices(nb_vertices);
    M.geo().coords.resize(nb_vertices);

    int index = 0;
    double x = 0.;
    getline(f, line); // first vertex
    while (line != "$EndNodes")
    {
      iss.clear();
      iss.str(line);
      iss >> index;
      index--; // gmesh start indices at 1
      for (int i = 0; i < M.dim_geo; ++i)
      {
        iss >> x;
        M.geo().coords[index][i] = x;
      }
      getline(f, line);
    }
    go_to_keyword(f, "$Elements");
    getline(f, line); //$Elements
    getline(f, line); //$nb elements
    int dim_el;
    int type = 0;
    int tags = 0;
    while (line != "$EndElements")
    {
      iss.clear();
      iss.str(line);
      iss >> trash >> type; // last trash is nb of tag (not used here)
      iss >> trash; //trash = nb of tags
      for(int i=0; i<trash; ++i){iss>>tags;} 
      dim_el = gtypes_dim[type];
      // Only add elements that belong to the mesh domain (ignore boundary elements for now)
      if(dim_el == M.dim_topo) {
        f_builders[static_cast<int>(gtypes_types[type])](M, iss);
        M.topo().shape_type().push_back(gtypes_types[type]);
      }
      getline(f, line);
    }

    M.topo().set_nb_cells(M.topo().template get_connectivities<M.dim_topo,0>().size());
  
  }

 
};

template<typename Mesh>
struct vtu
{
  static int vtk_cell_type(shapes::ShapeType shape)
  {
    switch (shape)
    {
    case shapes::ShapeType::vertex:       return 1;
    case shapes::ShapeType::edge:         return 3;
    case shapes::ShapeType::triangle:     return 5;
    case shapes::ShapeType::quadrangle:   return 9;
    case shapes::ShapeType::tetrahedron:  return 10;
    case shapes::ShapeType::hexahedron:   return 12;
    case shapes::ShapeType::prism:        return 13;
    case shapes::ShapeType::pyramid:      return 14;
    case shapes::ShapeType::count:        break;
    }
    throw std::runtime_error("vtu::write : unsupported cell shape");
  }

  //meshfile does not contain the extension
  static void write(const Mesh &M, std::string meshfile)
  {
    if(parallel::is_root() && parallel::size()>1)
    {
      std::ofstream pvtu(meshfile + ".pvtu");
      if (!pvtu)
      {
        throw std::runtime_error("vtu::write : cannot open file " + meshfile + ".pvtu");
      }

      pvtu << "<?xml version=\"1.0\"?>\n"
           << "<VTKFile type=\"PUnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
           << "  <PUnstructuredGrid GhostLevel=\"0\">\n";
       pvtu << "    <PPointData/>\n"
         << "    <PCellData Scalars=\"rank\">\n"
         << "      <PDataArray type=\"Int32\" Name=\"rank\" format=\"ascii\"/>\n"
         << "    </PCellData>\n"
         << "    <PPoints>\n"
         << "      <PDataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\"/>\n"
         << "    </PPoints>\n"
         << "    <PCells>\n"
         << "      <PDataArray type=\"Int64\" Name=\"connectivity\" format=\"ascii\"/>\n"
         << "      <PDataArray type=\"Int64\" Name=\"offsets\" format=\"ascii\"/>\n"
         << "      <PDataArray type=\"UInt8\" Name=\"types\" format=\"ascii\"/>\n"
         << "    </PCells>\n";
      const auto piece_prefix = std::filesystem::path(meshfile).filename().string();
      for (int rank = 0; rank < parallel::size(); ++rank)
      {
        pvtu << "    <Piece Source=\"" << piece_prefix << "_" << rank << ".vtu\"/>\n";
      }
      pvtu << "  </PUnstructuredGrid>\n"
           << "</VTKFile>\n";
    }
    meshfile += "_"+std::to_string(parallel::rank()) + ".vtu";
    std::ofstream f(meshfile);
    if (!f)
    {
      throw std::runtime_error("vtu::write : cannot open file " + meshfile);
    }

    const auto &cells = M.topo().template get_connectivities<Mesh::dim_topo,0>();
    if (M.topo().shape_type().size() != cells.size())
    {
      throw std::runtime_error("vtu::write : cell shape count does not match connectivity count");
    }

    f << "<?xml version=\"1.0\"?>\n"
      << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
      << "  <UnstructuredGrid>\n"
      << "    <Piece NumberOfPoints=\"" << M.geo().coords.size()
      << "\" NumberOfCells=\"" << cells.size() << "\">\n";

    f << "      <Points>\n"
      << "        <DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    for (const auto &point : M.geo().coords)
    {
      for (int dimension = 0; dimension < 3; ++dimension)
      {
        f << (dimension < Mesh::dim_geo ? point[dimension] : 0.0) << ' ';
      }
      f << '\n';
    }
    f << "        </DataArray>\n"
      << "      </Points>\n"
      << "      <Cells>\n"
      << "        <DataArray type=\"Int64\" Name=\"connectivity\" format=\"ascii\">\n";
    for (auto && c : M.cells())
    {
      for (auto && v : c.vertices())
      {
        f << v.index() << ' ';
      }
      f << '\n';
    }
    f << "        </DataArray>\n"
      << "        <DataArray type=\"Int64\" Name=\"offsets\" format=\"ascii\">\n";
    size_t offset = 0;
    for (size_t cell = 0; cell < cells.size(); ++cell)
    {
      offset += cells[cell].size();
      f << offset << ' ';
    }
    f << "\n        </DataArray>\n"
      << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
    for (auto shape : M.topo().shape_type())
    {
      f << vtk_cell_type(shape) << ' ';
    }
    f << "\n        </DataArray>\n"
      << "      </Cells>\n"
      << "      <CellData Scalars=\"rank\">\n"
      << "        <DataArray type=\"Int32\" Name=\"rank\" format=\"ascii\">\n";
    for (size_t cell = 0; cell < cells.size(); ++cell)
    {
      f << parallel::rank() << ' ';
    }
    f << "\n        </DataArray>\n"
      << "      </CellData>\n"
      << "    </Piece>\n"
      << "  </UnstructuredGrid>\n"
      << "</VTKFile>\n";
  }
};


#endif //IO_H