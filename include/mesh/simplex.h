#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <iostream>
#include <vector>
#include <array>
#include <macros.h>

//We define here only the topological objects that are the simplices. Nothing related yet with a mesh

/*!
\brief A simplex of D dimension is a polygon of D+1 vertices
*/
template <int D>
struct simplex
{
  static constexpr int nb_sub_included[5] = {}; /**< nb_sub_included[d] is the number of simplices of dimension d contained in the simplex */
};

#define SIMPLEX(D, IS)                                  \
  template <>                                           \
  struct simplex<D>                                     \
  {                                                     \
    static constexpr int nb_sub_included[5] = IS; \
  };



SIMPLEX(0, MYARRAY(1, 0, 0, 0, 0))
SIMPLEX(1, MYARRAY(2, 1, 0, 0, 0))
SIMPLEX(2, MYARRAY(3, 3, 1, 0, 0))
SIMPLEX(3, MYARRAY(4, 6, 4, 1, 0))
SIMPLEX(4, MYARRAY(5, 10, 10, 5, 1))

typedef simplex<0> vertex;
typedef simplex<1> edge;
typedef simplex<2> triangle;
typedef simplex<3> tetrahedron;
typedef simplex<4> pentachoron;



#endif // SIMPLEX_H