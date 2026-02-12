#ifndef MESH_TOPOLOGY_TOPOLOGY
#define MESH_TOPOLOGY_TOPOLOGY

#include <vector>

#include <mesh/topology/shape.h>

/*!
\brief The topology of the mesh is set of the connectivities between the different shapes
*/
template<int D>
class topology
{
  public:
    void set_nb_vertices(size_t n){nb_vertices = n;}

    size_t nb_vertices;
    size_t nb_cells;

    //shape types
    // shape_type[i] gives the type of the shape i. This shape is a cell (dim D).
    std::vector<ShapeType> shape_type;

    //Incidences     std::vector<int*> ???
    std::vector<std::vector<int>> cell_vertex; //Cell_vertex[i] gives the list of indices of the vertices incident to cell i
    std::vector<std::vector<int>> facet_vertex; //Cell_vertex[i] gives the list of indices of the vertices incident to cell i
    std::vector<std::vector<int>> edge_vertex; //Cell_vertex[i] gives the list of indices of the vertices incident to cell i

    //Adjacence

};



#endif