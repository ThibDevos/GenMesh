#ifndef MESH_GEOMETRY_GEOMETRY
#define MESH_GEOMETRY_GEOMETRY

#include <array>
#include <vector>

/*!
\brief The geometry of the mesh defines the coordinates
*/
template<int G>
class geometry
{
  public:
  std::vector<std::array<double,G>> coords;
};

#endif