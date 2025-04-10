#ifndef IO_H
#define IO_H

#include<string>
#include <fstream>
#include <sstream>
#include <mesh/mesh.h>
#include <file_functions.h>

// XXX works only for triangles
template<template<int> typename T, int G, int D>
void read_gmsh(mesh< T, G, D> & M, std::string meshfile)
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

  getline(f, line); // Number of nodes
  iss.str(line);
  int nb_nodes;
  iss >> nb_nodes;
  M.topo.V.resize(nb_nodes);
  M.geo.coords.resize(nb_nodes);
  M.nb_nodes = nb_nodes;

  int index = 0;
  double x = 0;
  getline(f, line); // first node
  while (line != "$EndNodes")
  {
    iss.clear();
    iss.str(line);
    iss >> index;
    index--; // gmesh start indices at 1
    M.topo.V[index].set_index(index);
    getline(f, line);
    for (int i = 0; i < G; ++i)
    {
      iss >> x;
      M.geo.coords[index][i] = x;
    }
  }

  // while(line!="$Elements"){getline(f,line);}
  go_to_keyword(f, "$Elements");
  int nb_cells = 0;
  int type = 0;
  int tags = 0;
  while (line != "$EndElements")
  {
    iss.clear();
    iss.str(line);
    iss >> trash >> type; // last trash is nb of tag (not used here)
    if (type == gmesh_types<T>::types[D])
    {
      nb_cells++;
    }
    getline(f, line);
  }
  M.nb_cells = nb_cells;
  M.topo.C.resize(nb_cells);
  M.topo.C_CV.resize(nb_cells);
  go_to_keyword(f, "$Elements");
  getline(f, line);
  index = 0;
  type = 0;
  int index_v = 0;
  std::array<entity<T, 0> *, T<D>::nb_sub_included[0]> incident_nodes;
  while (line != "$EndElements")
  {
    iss.clear();
    iss.str(line);
    iss >> trash >> type >> tags; // last trash is nb of tag (not used here)
    for (int i = 0; i < tags; ++i)
    {
      iss >> trash;
    }
    if (type == gmesh_types<T>::types[D])
    {
      M.topo.C[index].set_index(index);
      for (int i = 0; i < T<D>::nb_sub_included[0]; ++i)
      {
        iss >> index_v;
        incident_nodes[i] = &M.topo.V[index_v - 1];
        // iss >> incident_nodes[i];
        // incident_nodes[i] --;
      }
      // topo.C[index].template set_incidence<0>(incident_nodes);
      M.topo.C_CV[index] = incident_nodes;
      index++;
    }
    getline(f, line);
  }
}

// XXX works only for triangle
template<template<int> typename T, int G, int D>
void print_vtu(mesh< T, G, D> & M, std::string meshfile)
{
  std::ofstream f;
  f.open(meshfile);
  f<<"<?xml version=\"1.0\"?> "<<std::endl;
  f<< "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\">" <<std::endl;
  f <<"<UnstructuredGrid> "<<std::endl;
  f <<"<Piece NumberOfPoints=\""<<M.nb_nodes<<"\" NumberOfCells=\""<<M.nb_cells<<"\">"<<std::endl;
  f<<"<Points>"<<std::endl;
  f<<"<DataArray type=\"Float64\" NumberOfComponents=\""<<3<<"\">"<<std::endl;
  for(auto c : M.geo.coords)
  {
    if constexpr (G==1){f<<c[0]<<" "<<0.<<" "<<0.<<std::endl;}
    else if constexpr (G==2){f<<c[0]<<" "<<c[1]<<" "<<0.<<std::endl;}
    else {f<<c[0]<<" "<<c[1]<<" "<<c[2]<<std::endl;}
  }
  f<<"</DataArray>"<<std::endl;
  f<<"</Points>"<<std::endl;
  f<<"<Cells>"<<std::endl;
  f<<"<DataArray type=\"Int32\" Name=\"connectivity\">"<<std::endl;
  int n = T<D>::nb_sub_included[0]; //nb vertices per cell
  for(auto c : M.topo.C_CV)
  {
    for(int i = 0; i < n; ++i)
    {
      f<<c[i]->get_index()<<" ";
    }
    f<<std::endl;
  }
  f<<"</DataArray>"<<std::endl;
  f<<"<DataArray type=\"Int32\" Name=\"offsets\"> "<<std::endl;
  int nn = n;
  for(int i = 0; i<M.nb_cells; ++i)
  {
    f<<nn<<" "<<std::endl;
    nn += n;
  }
  f<<"</DataArray>"<<std::endl;
  f<<"<DataArray type=\"Int32\" Name=\"types\">" <<std::endl;
  for(int i = 0; i<M.nb_cells; ++i)
  {
    f<<12<<" "<<std::endl;
  }
  f<<"</DataArray>"<<std::endl;
  f<<"</Cells>"<<std::endl;
  f<<"</Piece> "<<std::endl;
  f<<"</UnstructuredGrid> "<<std::endl;
  f<<"</VTKFile>"<<std::endl;
  f.close();
}
#endif //IO_H