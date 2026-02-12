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
    void set_nb_vertices(size_t n){nb_vertices_ = n;}
    void set_nb_cells(size_t n){nb_cells_ = n;}
    void set_nb_facets(size_t n){nb_facets_ = n;}
    void set_nb_edges(size_t n){nb_edges_ = n;}

    size_t nb_vertices(){return nb_vertices_;}
    size_t nb_cells(){return nb_cells_;}
    size_t nb_facets(){return nb_facets_;}
    size_t nb_edges(){return nb_edges_;}

    size_t nb_vertices_;
    size_t nb_cells_;
    size_t nb_facets_;
    size_t nb_edges_;

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