#ifndef MESH_TOPOLOGY_SHAPE
#define MESH_TOPOLOGY_SHAPE
#include <iostream>

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
  static constexpr int edge_vertex[3][2] = {{0,1}, {1,2}, {2,0}};
};
struct tetrahedron
{
  static constexpr int D = 3;
  static constexpr int nb_sub_included[5] = {4,6,4,1,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
  static constexpr int edge_vertex[6][2] = {{0,1}, {0,2}, {0,3}, {1,2}, {1,3}, {2,3}};
  static constexpr int face_vertex_data[12] = {0,1,2, 0,1,3, 0,2,3, 1,2,3};
  static constexpr int face_offsets[5] = {0, 3, 6, 9, 12};
};
struct quadrangle
{
  static constexpr int D = 2;
  static constexpr int nb_sub_included[5] = {4,4,1,0,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
  static constexpr int edge_vertex[4][2] = {{0,1}, {1,2}, {2,3}, {3,0}};
};
struct hexahedron
{
  static constexpr int D = 3;
  static constexpr int nb_sub_included[5] = {8,12,6,1,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
  static constexpr int edge_vertex[12][2] = {{0,1}, {1,2}, {2,3}, {3,0}, {4,5}, {5,6}, {6,7}, {7,4}, {0,4}, {1,5}, {2,6}, {3,7}};
  static constexpr int face_vertex_data[24] = {0,1,5,4, 1,2,6,5, 2,3,7,6, 3,0,4,7, 0,3,2,1, 4,5,6,7};
  static constexpr int face_offsets[7] = {0, 4, 8, 12, 16, 20, 24};
};
struct prism
{
  static constexpr int D = 3;
  static constexpr int nb_sub_included[5] = {6,9,5,1,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
  static constexpr int edge_vertex[9][2] = {{0,1}, {1,2}, {2,0}, {3,4}, {4,5}, {5,3}, {0,3}, {1,4}, {2,5}};
  static constexpr int face_vertex_data[18] = {0,1,4,3, 1,2,5,4, 2,0,3,5, 0,1,2, 3,4,5};
  static constexpr int face_offsets[6] = {0, 4, 8, 12, 15, 18};
};
struct pyramid
{
  static constexpr int D = 3;
  static constexpr int nb_sub_included[5] = {5,8,5,1,0}; /**< nb_sub_included[d] is the number of shapes of dimension d contained in the shape */
  static constexpr int edge_vertex[8][2] = {{0,1}, {1,2}, {2,3}, {3,0}, {0,4}, {1,4}, {2,4}, {3,4}};
  static constexpr int face_vertex_data[16] = {0,1,2,3, 0,1,4, 1,2,4, 2,3,4, 3,0,4};
  static constexpr int face_offsets[6] = {0, 4, 7, 10, 13, 16};
};

enum class ShapeType {vertex, edge, triangle, tetrahedron, quadrangle, hexahedron, prism, pyramid, count};

namespace shapes
{
  struct edge_descriptor {
    const int (*edge_vertex)[2];
    const int nb_edge;
  };
  inline static constexpr edge_descriptor edge_desc [] = 
  {
    {nullptr, 0},
    {nullptr, 0},
    {triangle::edge_vertex, triangle::nb_sub_included[1]},
    {tetrahedron::edge_vertex, tetrahedron::nb_sub_included[1]},
    {quadrangle::edge_vertex, quadrangle::nb_sub_included[1]},
    {hexahedron::edge_vertex, hexahedron::nb_sub_included[1]},
    {prism::edge_vertex, prism::nb_sub_included[1]},
    {pyramid::edge_vertex, pyramid::nb_sub_included[1]}
  };

  struct face_descriptor {
    const int *face_vertex;
    const int *face_vertex_offset;
    const int nb_face;
  };
  inline static constexpr face_descriptor face_desc [] = 
  {
    {nullptr, 0},
    {nullptr, 0},
    {nullptr, 0},
    {tetrahedron::face_vertex_data,tetrahedron::face_offsets, tetrahedron::nb_sub_included[2]},
    {nullptr, 0},
    {hexahedron::face_vertex_data, hexahedron::face_offsets, hexahedron::nb_sub_included[2]},
    {prism::face_vertex_data, prism::face_offsets, prism::nb_sub_included[2]},
    {pyramid::face_vertex_data, pyramid::face_offsets, pyramid::nb_sub_included[2]}
  };


  ShapeType get_shape_from_dim_vertices(size_t dim, size_t nb_vert)
  {
    if(dim==0) return ShapeType::vertex;
    if(dim==1) return ShapeType::edge;
    if(dim==2)
    {
      switch (nb_vert)
      {
      case 3:
        return ShapeType::triangle;
        break;
      case 4:
        return ShapeType::quadrangle;
        break;
      }
    }
    if(dim==3)
    {
      switch (nb_vert)
      {
      case 4:
        return ShapeType::tetrahedron;
        break;
      case 5:
        return ShapeType::pyramid;
        break;
      case 6:
        return ShapeType::prism;
        break;
      }
    }
    return ShapeType::count;
  }
};

#endif