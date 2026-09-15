#include <cassert>
#include <iostream>
#include <vector>

#include <mesh/mesh.h>
#include <io.h>
#include <core/log.h>
#include <connectivities.h>
#include <connectivities_hybrid.h>
#include <entity_view_expected_values.h>

using namespace bib;

template <size_t D, size_t D1, size_t D2>
void test_topology_connectivities(
    topology<D> & topo_,
    std::vector<std::vector<size_t>> const & expected)
{
  topo_.template get_connectivities<D1, D2>(0); //need to build the connectivities
  assert(expected.size() == topo_.nb_entities(D1));

  for (size_t i = 0; i < expected.size(); ++i)
  {
    auto result = topo_.template get_connectivities<D1, D2>(i);
    assert(result.size()==expected[i].size());
    for(size_t j=0; j<result.size(); ++j)
    {
      std::cout<<result[j]<<" "<<expected[i][j]<<std::endl;
    }
    std::cout<<std::endl;
    assert(std::equal(result.begin(), result.end(), expected[i].begin(), expected[i].end()));
  }
  message("finished");
  std::cout<<std::endl;
}

int main()
{
  message("Test 1D connectivities");
  
  {
    message("C1D_0_0");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_topology_connectivities<1, 0, 0>(M1.topo(), C1D_0_0);
  }
  {
    message("C1D_0_1");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_topology_connectivities<1, 0, 1>(M1.topo(), C1D_0_1);
  }
  {
    message("C1D_1_0");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_topology_connectivities<1, 1, 0>(M1.topo(), C1D_1_0);
  }
  {
    message("C1D_1_1");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_topology_connectivities<1, 1, 1>(M1.topo(), C1D_1_1);
  }
  std::cout << std::endl;
  message("Test 2D connectivities simplices");

  {
    message("C2D_0_0");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 0, 0>(M2.topo(), C2D_0_0);
  }
  {
    message("C2D_0_1");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 0, 1>(M2.topo(), C2D_0_1);
  }
  {
    message("C2D_0_2");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 0, 2>(M2.topo(), C2D_0_2);
  }
  {
    message("C2D_1_0");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 1, 0>(M2.topo(), C2D_1_0);
  }
  {
    message("C2D_1_1");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 1, 1>(M2.topo(), C2D_1_1);
  }
  {
    message("C2D_1_2");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 1, 2>(M2.topo(), C2D_1_2);
  }
  {
    message("C2D_2_0");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 2, 0>(M2.topo(), C2D_2_0);
  }
  {
    message("C2D_2_1");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 2, 1>(M2.topo(), C2D_2_1);
  }
  {
    message("C2D_2_2");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 2, 2>(M2.topo(), C2D_2_2);
  }

  std::cout << std::endl;
  message("Test 2D connectivities hybrid");
  {
    message("C2D_0_0 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 0, 0>(M2.topo(), C2D_0_0_hybrid);
  }
  {
    message("C2D_0_1 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 0, 1>(M2.topo(), C2D_0_1_hybrid);
  }
  {
    message("C2D_0_2 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 0, 2>(M2.topo(), C2D_0_2_hybrid);
  }
  {
    message("C2D_1_0 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 1, 0>(M2.topo(), C2D_1_0_hybrid);
  }
  {
    message("C2D_1_1 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 1, 1>(M2.topo(), C2D_1_1_hybrid);
  }
  {
    message("C2D_1_2 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 1, 2>(M2.topo(), C2D_1_2_hybrid);
  }
  {
    message("C2D_2_0 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 2, 0>(M2.topo(), C2D_2_0_hybrid);
  }
  {
    message("C2D_2_1 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 2, 1>(M2.topo(), C2D_2_1_hybrid);
  }
  {
    message("C2D_2_2 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 2, 2>(M2.topo(), C2D_2_2_hybrid);
  }

  message("Test 3D connectivities");
  {
    message("C3D_0_0");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 0, 0>(M3.topo(), C3D_0_0);
  }
  {
    message("C3D_0_1");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 0, 1>(M3.topo(), C3D_0_1);
  }
  {
    message("C3D_0_2");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 0, 2>(M3.topo(), C3D_0_2);
  }
  {
    message("C3D_0_3");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 0, 3>(M3.topo(), C3D_0_3);
  }

  {
    message("C3D_1_0");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 1, 0>(M3.topo(), C3D_1_0);
  }
  {
    message("C3D_1_1");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 1, 1>(M3.topo(), C3D_1_1);
  }
  {
    message("C3D_1_2");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 1, 2>(M3.topo(), C3D_1_2);
  }
  {
    message("C3D_1_3");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 1, 3>(M3.topo(), C3D_1_3);
  }

  {
    message("C3D_2_0");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 2, 0>(M3.topo(), C3D_2_0);
  }

  {
    message("C3D_2_1");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 2, 1>(M3.topo(), C3D_2_1);
  }
  {
    message("C3D_2_2");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 2, 2>(M3.topo(), C3D_2_2);
  }
  {
    message("C3D_2_3");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 2, 3>(M3.topo(), C3D_2_3);
  }

  {
    message("C3D_3_0");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 3, 0>(M3.topo(), C3D_3_0);
  }

  {
    message("C3D_3_1");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 3, 1>(M3.topo(), C3D_3_1);
  }

  {
    message("C3D_3_2");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 3, 2>(M3.topo(), C3D_3_2);
  }

  {
    message("C3D_3_3");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 3, 3>(M3.topo(), C3D_3_3);
  }

  std::cout<<std::endl;
  message("Test 3D connectivities hybrid");
  {
    {
      message("C3D_0_0 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 0, 0>(M3.topo(), C3D_0_0_hybrid);
    }
    {
      message("C3D_0_1 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 0, 1>(M3.topo(), C3D_0_1_hybrid);
    }
    {
      message("C3D_0_2 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 0, 2>(M3.topo(), C3D_0_2_hybrid);
    }
    {
      message("C3D_0_3 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 0, 3>(M3.topo(), C3D_0_3_hybrid);
    }
    {
      message("C3D_1_0 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 1, 0>(M3.topo(), C3D_1_0_hybrid);
    }
    {
      message("C3D_1_1 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 1, 1>(M3.topo(), C3D_1_1_hybrid);
    }
    {
      message("C3D_1_2 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 1, 2>(M3.topo(), C3D_1_2_hybrid);
    }
    {
      message("C3D_1_3 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 1, 3>(M3.topo(), C3D_1_3_hybrid);
    }
    {
      message("C3D_2_0 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 2, 0>(M3.topo(), C3D_2_0_hybrid);
    }
    {
      message("C3D_2_1 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 2, 1>(M3.topo(), C3D_2_1_hybrid);
    }
    {
      message("C3D_2_2 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 2, 2>(M3.topo(), C3D_2_2_hybrid);
    }
    {
      message("C3D_2_3 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 2, 3>(M3.topo(), C3D_2_3_hybrid);
    }
    {
      message("C3D_3_0 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 3, 0>(M3.topo(), C3D_3_0_hybrid);
    }
    {
      message("C3D_3_1 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 3, 1>(M3.topo(), C3D_3_1_hybrid);
    }
    {
      message("C3D_3_2 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 3, 2>(M3.topo(), C3D_3_2_hybrid);
    }
    {
      message("C3D_3_3 hybrid");
      mesh<3> M3;
      gmesh<mesh<3>> G3;
      G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
      test_topology_connectivities<3, 3, 3>(M3.topo(), C3D_3_3_hybrid);
    }
  }
  std::cout<<std::endl;
  message("Test entity view");
  { 
    message("expected_1d_vertices");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_entity_view<1, 5, 0>(M1, expected_1d_vertices);
  }
  { 
    message("expected_1d_edges");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_entity_view<1, 4, 1>(M1, expected_1d_edges);
  }
  { 
    message("expected_2d_hybrid_vertices");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_entity_view<2, 10, 0>(M2, expected_2d_hybrid_vertices);
  }
  { 
    message("expected_2d_hybrid_edges");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_entity_view<2, 18, 1>(M2, expected_2d_hybrid_edges);
  }
  { 
    message("expected_2d_hybrid_cells");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_entity_view<2, 9, 2>(M2, expected_2d_hybrid_cells);
  }
  { 
    message("expected_3d_hybrid_vertices");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_entity_view<3, 15, 0>(M3, expected_3d_hybrid_vertices);
  }
  { 
    message("expected_3d_hybrid_edges");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_entity_view<3, 30, 1>(M3, expected_3d_hybrid_edges);
  }
  { 
    message("expected_3d_hybrid_faces");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_entity_view<3, 21, 2>(M3, expected_3d_hybrid_faces);
  }
  { 
    message("expected_3d_hybrid_cells");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_entity_view<3, 5, 3>(M3, expected_3d_hybrid_cells);
  }
  
  return 0;
}