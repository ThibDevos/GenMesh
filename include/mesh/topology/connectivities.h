#ifndef MESH_TOPOLOGY_CONNECTIVITIES
#define MESH_TOPOLOGY_CONNECTIVITIES

#include <iostream>
#include <vector>
#include <set>

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
  void build_connectivities(std::array<size_t, D+1> const & nb_entities)
  {
    std::cout<<"build connectivities " <<D1<<" "<<D2<<std::endl;
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
    else // C[D1][D2] = C[D1][k] \circ C[k][D2]. We use k=0 as the relations D->0 are already done
    {
      if(C[0][D2].size()==0 && C[D2][0].size()!=0)
      {
        C[0][D2].resize(nb_entities[0]);
        build_connectivities<0,D2>(nb_entities);
      }
      if(C[D1][0].size()==0 && C[0][D1].size()!=0)
      {
        C[D1][0].resize(nb_entities[D1]);
        build_connectivities<D1,0>(nb_entities);
      }
      for(int i=0; i<C[D1][0].size(); ++i) //i is the index of the entity of dim D1
      {
        std::set<size_t> set;
        for(int j=0; j<C[D1][0][i].size(); ++j)
        {
          size_t j_global = C[D1][0][i][j]; //j_global is the global index of the jth vertex of entity i
          for(int k=0; k<C[0][D2][j_global].size(); ++k)
          {
            size_t k_global = C[0][D2][j_global][k];
            set.insert(k_global);
            }
          }
          C[D1][D2][i].assign(set.begin(), set.end());
      }
    }
  }


};

#endif