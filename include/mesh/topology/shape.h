#ifndef MESH_TOPOLOGY_SHAPE
#define MESH_TOPOLOGY_SHAPE
#include <iostream>
#include <macros.h>

struct vertex
{
  static constexpr int D = 0;
  static constexpr int nb_sub_included[5] = {1,0,0,0,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
};
struct edge
{
  static constexpr int D = 1;
  static constexpr int nb_sub_included[5] = {2,1,0,0,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
};
struct triangle
{
  static constexpr int D = 2;
  static constexpr int nb_sub_included[5] = {3,3,1,0,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
};
struct tetrahedron
{
  static constexpr int D = 3;
  static constexpr int nb_sub_included[5] = {4,6,3,1,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
};
struct quadrangle
{
  static constexpr int D = 2;
  static constexpr int nb_sub_included[5] = {0,0,0,0,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
};
struct hexahedron
{
  static constexpr int D = 3;
  static constexpr int nb_sub_included[5] = {0,0,0,0,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
};
struct prism
{
  static constexpr int D = 3;
  static constexpr int nb_sub_included[5] = {0,0,0,0,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
};
struct pyramid
{
  static constexpr int D = 3;
  static constexpr int nb_sub_included[5] = {0,0,0,0,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
};

enum class ShapeType {vertex, edge, triangle, tetrahedron, quadrangle, hexahedron, prism, pyramid, count};



#endif