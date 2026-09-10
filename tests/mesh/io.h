#ifndef MESH_IO_TESTS_H
#define MESH_IO_TESTS_H

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>

#include <io.h>

inline void test_vtu_write(const std::string &mesh_file, const std::string &output_file)
{
  mesh<2> M;
  gmesh<mesh<2>> reader;
  reader.read_gmsh(M, mesh_file);
  vtu<mesh<2>>::write(M, output_file);

  std::ifstream output(output_file);
  assert(output.good());
  const std::string xml((std::istreambuf_iterator<char>(output)),
                        std::istreambuf_iterator<char>());

  assert(xml.find("<VTKFile type=\"UnstructuredGrid\"") != std::string::npos);
  assert(xml.find("<Piece NumberOfPoints=\"10\" NumberOfCells=\"9\">") != std::string::npos);
  assert(xml.find("Name=\"connectivity\"") != std::string::npos);
  assert(xml.find("Name=\"offsets\"") != std::string::npos);
  assert(xml.find("Name=\"types\"") != std::string::npos);

  std::remove(output_file.c_str());
}

#endif // MESH_IO_TESTS_H
