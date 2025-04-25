#ifndef ENTITY
#define ENTITY

#include <numeric>
#include <cassert>
#include <mesh/simplex.h>


/*!
\brief An entity of the mesh can be:
- a vertex (dim 0)
- an edge (dim 1)
- a face or cell (dime 2)
- a cell (dim 3)
*/
template <template<int> typename T, int D> 
class entity
{
  public:
    void set_index(size_t i){this->index = i;}
    int get_index() const {return index;}
    void print_entity(){std::cout<<"entity of dim "<<D<<". Index: "<<index<<std::endl;} //will be removed

    template<int D0>
    void set_incidence(std::array<int,T<D>::nb_sub_included[D0]> incident_entities)
    {
      int ii = 0;
      for(int i = incidence_offsets[D0]; i < incidence_offsets[D0] + T<D>::nb_sub_included[D0]; ++i)
      {
        incidence[i] = incident_entities[ii];
        ++ii;
      }
    }

    template <int D0>
    int get_incidence(int i) const
    {
      assert(i<T<D>::nb_sub_included[D0]);
      return incidence[incidence_offsets[D0] + i];
    }
  private:
    size_t index = 0;
    size_t incidence_offsets[5] = {0,T<D>::nb_sub_included[0],std::accumulate(&(T<D>::nb_sub_included[0]),&(T<D>::nb_sub_included[1]),0),
    std::accumulate(&(T<D>::nb_sub_included[0]),&(T<D>::nb_sub_included[2]),0),std::accumulate(&(T<D>::nb_sub_included[0]),&(T<D>::nb_sub_included[3]),0)};
    size_t incidence[std::accumulate(&(T<D>::nb_sub_included[0]),&(T<D>::nb_sub_included[4]),0)];
};

#endif //ENTITY