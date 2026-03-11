#ifndef MESH_TOPOLOGY_INCIDENCE
#define MESH_TOPOLOGY_INCIDENCE

using  relation_table = std::vector<std::vector<int>>;

/*!
\brief Incidence relations of the entities of dim D

This structure contains tables of incidences relations for an entity of dimension D
As there are 4 level of entities (vertex, edge, cell and facet), there are only 3 tables.
They are called lowest, highest and middle which correspond respictively to the relations 
with the lowest, highest dimension, and the third
*/
template<size_t D>
struct incidence
{
 

  relation_table lowest;
  relation_table highest;
  relation_table middle;

};



#endif //MESH_TOPOLOGY_INCIDENCE