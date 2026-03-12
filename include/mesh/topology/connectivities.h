#ifndef MESH_TOPOLOGY_CONNECTIVITIES
#define MESH_TOPOLOGY_CONNECTIVITIES

#include <iostream>
#include <vector>

/*! \brief stores the connectivities of the topology
connectivities[k][l] gives the relation table of entities of dim k containing entities of dim l
for exemple connectivities[D][0][i] gives the indices of the vertices composing the cell i
The diagonal represent the strong adjacency relations i.e. connectitivites[k][k][i] 
gives the indices of the entities of dim k that share an entity of dim k-1 with the entity i of dim k
*/
template<size_t D>
struct Connectivities
{
  using  relation_table = std::vector<std::vector<size_t>>;
  relation_table C[D+1][D+1]; 

 
  /*!
  Build connectivities C[D1][D2]
  */
  template<size_t D1, size_t D2>
  void build_connectivities()
  {
    std::cout<<"build connectivities"<<std::endl;
    // std::cout<<"Size C[D1][D2]: "<<C[D1][D2].size()<<std::endl;
    if(C[D2][D1].size()!=0)
    {
      for(int i = 0; i<C[D2][D1].size(); ++i)
      {
        for(int j=0; j<C[D2][D1][i].size(); ++j)
        {
          size_t global_j = C[D2][D1][i][j];
          // std::cout<<global_j<<std::endl;
          C[D1][D2][global_j].push_back(i);
        }
      }
    } 
  }


};

#endif