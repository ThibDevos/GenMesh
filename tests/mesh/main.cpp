#include <cassert>
#include <iostream>
#include <vector>

#include <mesh/mesh.h>
#include <io.h>
#include <core/log.h>
#include <connectivities.h>

using namespace bib;

template <size_t D, size_t D1, size_t D2>
void test_topology_connectivities(
    topology<D> & topo,
    std::vector<std::vector<size_t>> const & expected)
{
  topo.template get_connectivities<D1, D2>(0); //need to build the connectivities
  std::cout<<expected.size() <<" "<< topo.nb_entities(D1)<<std::endl;
  assert(expected.size() == topo.nb_entities(D1));

  for (size_t i = 0; i < expected.size(); ++i)
  {
    std::cout<<i<<std::endl;
    std::vector<size_t> const & result = topo.template get_connectivities<D1, D2>(i);
    for(int j=0; j<result.size(); ++j)
    {
      std::cout<<result[j]<<" "<<expected[i][j]<<std::endl;
    }
    std::cout<<std::endl;
    assert(result == expected[i]);
  }
}

int main()
{
  mesh<1> M1;
  gmesh<mesh<1>> G1;
  G1.read_gmsh(M1,std::string(TEST_FILES_DIR)+"/test_1d.msh");
  message("Test 1D connectivities");
  message("C1D_1_0");
  test_topology_connectivities<1, 1, 0>(M1.topo, C1D_1_0);
  message("C1D_0_1");
  test_topology_connectivities<1, 0, 1>(M1.topo, C1D_0_1);
  std::cout<<std::endl;
  mesh<2> M2;
  gmesh<mesh<2>> G2;
  G2.read_gmsh(M2,std::string(TEST_FILES_DIR)+"/test_2d.msh");
  message("Test 2D connectivities");
  message("C2D_1_0");
  test_topology_connectivities<2, 1, 0>(M2.topo, C2D_1_0);
  message("C2D_0_1");
  test_topology_connectivities<2, 0, 1>(M2.topo, C2D_0_1);
  message("C2D_2_0");
  test_topology_connectivities<2, 2, 0>(M2.topo, C2D_2_0);
  message("C2D_0_2");
  test_topology_connectivities<2, 0, 2>(M2.topo, C2D_0_2);
  
  mesh<3> M3;
  gmesh<mesh<3>> G3;
  message("Test 3D connectivities");
  G3.read_gmsh(M3,std::string(TEST_FILES_DIR)+"/test2_3d.msh");
  message("C3D_1_0");
  test_topology_connectivities<3, 1, 0>(M3.topo, C3D_1_0);
  message("C3D_2_0");
  test_topology_connectivities<3, 2, 0>(M3.topo, C3D_2_0);
  message("C3D_0_1");
  test_topology_connectivities<3, 0, 1>(M3.topo, C3D_0_1);
  message("C3D_3_0");
  test_topology_connectivities<3, 3, 0>(M3.topo, C3D_3_0);
  message("C3D_0_3");
  test_topology_connectivities<3, 0, 3>(M3.topo, C3D_0_3);
  return 0;
}