#include <mesh/mesh.h>
#include <io.h>

#include <iomanip>
#include <iostream>
#include <string>

template <int G, int D>
void print_mesh_summary(const std::string &label, mesh<G, D> &M)
{
  std::cout << "\n== " << label << " ==\n";
  std::cout << "vertices: " << M.topo.nb_vertices() << "\n";
  std::cout << "cells: " << M.topo.nb_cells() << "\n";

  std::cout << "Vertex coordinates:\n";
  for (auto v : M.vertices())
  {
    auto coord = v.coordinates();
    std::cout << "  v" << v.index() << " -> (";
    for (size_t i = 0; i < coord[0].size(); ++i)
    {
      if (i != 0)
        std::cout << ", ";
      std::cout << std::fixed << std::setprecision(3) << coord[0][i];
    }
    std::cout << ")\n";
  }

  std::cout << "Cell measures:\n";
  for (auto c : M.cells())
  {
    std::cout << "  cell " << c.index() << " -> measure = " << c.measure() << "\n";
  }

  if constexpr (D >= 2)
  {
    std::cout << "Adjacency example (edge -> incident cells):\n";
    for (auto e : M.edges())
    {
      std::cout << "  edge " << e.index() << " -> cells: ";
      bool first = true;
      for (auto c : e.cells())
      {
        if (!first)
          std::cout << ", ";
        std::cout << c.index();
        first = false;
      }
      std::cout << "\n";
    }
  }
}

int main()
{
  {
    mesh<1> M1;
    gmesh<mesh<1>> G;
    G.read_gmsh(M1, "test_files/test_1d.msh");
    print_mesh_summary("1D mesh", M1);
  }

  {
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, "test_files/test_2d_hybrid.msh");
    print_mesh_summary("2D mesh", M2);
  }

  {
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, "test_files/test_3d_hybrid.msh");
    print_mesh_summary("3D mesh", M3);
  }

  return 0;
}