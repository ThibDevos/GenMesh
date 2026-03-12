#ifndef IO_H
#define IO_H

#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>

#include <mesh/mesh.h>
#include <mesh/topology/shape.h>
#include <file_functions.h>

template<typename Mesh>
struct gmesh
{
 std::map<int, int> gtypes_dim{
      {15, 0}, {1, 1}, {2, 2}, {3, 2}, {4, 3}, {5, 3}, {6, 3}, {7, 3}};
  std::map<int, ShapeType> gtypes_types{
      {15, ShapeType::vertex}, {1, ShapeType::edge}, {2, ShapeType::triangle}, {3, ShapeType::quadrangle}, {4, ShapeType::tetrahedron}, {5, ShapeType::hexahedron}, {6, ShapeType::prism}, {7, ShapeType::pyramid}};


  
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
    M.topo.connectivities.C[Shape::D][0].push_back(local_vertices);
    // switch (Shape::D)
    // {
    // case M.dim_topo: //cell
    //   M.topo.connectivi
    //   break;
    // case M.dim_topo - 1: //facet 
    //    M.topo.facet_connectivities.lowest.push_back(local_vertices);
    //   break;
    // case 1: //edge
    //    M.topo.edge_connectivities.lowest.push_back(local_vertices);
    //   break;
    
    // default:
    //   break;
    // }
  }

  using builders = void(*)(Mesh & M, std::istringstream & iss);

  static inline constexpr std::array<builders,
  static_cast<int>(ShapeType::count)> f_builders=  { //should follow the order of enum ShapeType in shape.h
    &build_element_from_vertices<vertex>,
    &build_element_from_vertices<edge>,
    &build_element_from_vertices<triangle>,
    &build_element_from_vertices<tetrahedron>,
    &build_element_from_vertices<quadrangle>,
    &build_element_from_vertices<hexahedron>,
    &build_element_from_vertices<prism>,
    &build_element_from_vertices<pyramid>
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
    std::istringstream iss;
    std::string line;
    std::ifstream f(meshfile);
    int trash = 0;

    if (!f)
    {
      std::runtime_error("mesh::read : file does not exist");
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
    std::cout<<"nb vertices : "<<nb_vertices<<std::endl;
    M.topo.set_nb_vertices(nb_vertices);
    M.geo.coords.resize(nb_vertices);

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
        M.geo.coords[index][i] = x;
      }
      getline(f, line);
    }
    go_to_keyword(f, "$Elements");
    getline(f, line); //$Elements
    getline(f, line); //$nb elements
    int nb_dim3 = 0;
    int nb_dim2 = 0;
    int nb_dim1 = 0;
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
      f_builders[static_cast<int>(gtypes_types[type])](M, iss);  
      getline(f, line);
    }

    M.topo.set_nb_cells(M.topo.connectivities.C[M.dim_topo][0].size());
    M.topo.set_nb_edges(M.topo.connectivities.C[1][0].size());
    if(M.dim_topo==3) //for dim_topo <3, the facets correspond to the edges or the vertices, so it is already set
      M.topo.set_nb_facets(M.topo.connectivities.C[M.dim_topo-1][0].size());
  }

 
};


#endif //IO_H