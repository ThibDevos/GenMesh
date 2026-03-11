#ifndef MESH_TOPOLOGY_TOPOLOGY
#define MESH_TOPOLOGY_TOPOLOGY

#include <array>
#include <vector>

#include <mesh/topology/shape.h>
#include <core/log.h>

/*!
\brief The topology of the mesh is set of the connectivities between the different shapes
*/
template<int D>
class topology
{
  using  relation_table = std::vector<std::vector<int>>;

  public:
    void set_nb_vertices(size_t n){nb_entities_[0] = n;}
    void set_nb_edges(size_t n){nb_entities_[1] = n;}
    void set_nb_facets(size_t n){nb_entities_[D-1] = n;}
    void set_nb_cells(size_t n){nb_entities_[D] = n;}

    size_t nb_vertices(){return nb_entities_[0];}
    size_t nb_edges(){return nb_entities_[1];}
    size_t nb_facets(){return nb_entities_[D-1];}
    size_t nb_cells(){return nb_entities_[D];}
    size_t nb_entities(size_t i){/*assert(i<D+1);*/ return nb_entities_[i];}

    std::array<size_t, D+1> nb_entities_;

    //shape types
    // shape_type[i] gives the type of the shape i. This shape is a cell (dim D).
    std::vector<ShapeType> shape_type;

    //Incidences
    relation_table connectivities[D+1][D+1]; //the diagonal is empty as it represents the adjacencies



    template<size_t D1, size_t D2>
    std::vector<int>& get_incidence(size_t index)
    {
      static_assert(D1!=D2);
      if(connectivities[D1][D1].size()==0)
      {
        bib::error("need to build the connectivities");
      }
      return connectivities[D1][D2][index];
    }
    

    //Adjacence

};



#endif