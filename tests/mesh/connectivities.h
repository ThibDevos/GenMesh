#ifndef TESTS_MESH_CONNECTIVITIES
#define TESTS_MESH_CONNECTIVITIES

#include<expected_data/expected_connectivities.h>
#include<expected_data/expected_connectivities_hybrid.h>

template <size_t D, size_t D1, size_t D2>
void test_topology_connectivities(
    topology<D> &topo,
    std::vector<std::vector<size_t>> const &expected)
{
  topo.template get_connectivities<D1, D2>(0); // need to build the connectivities
  assert(expected.size() == topo.nb_entities(D1));

  for (size_t i = 0; i < expected.size(); ++i)
  {
    std::vector<size_t> const &result = topo.template get_connectivities<D1, D2>(i);
    assert(result.size() == expected[i].size());
    assert(result == expected[i]);
  }
}

void test_connectivities()
{
  bib::message("Test 1D connectivities");
  {
    bib::message("C1D_0_0");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_topology_connectivities<1, 0, 0>(M1.topo, C1D_0_0);
  }
  {
    bib::message("C1D_0_1");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_topology_connectivities<1, 0, 1>(M1.topo, C1D_0_1);
  }
  {
    bib::message("C1D_1_0");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_topology_connectivities<1, 1, 0>(M1.topo, C1D_1_0);
  }
  {
    bib::message("C1D_1_1");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_topology_connectivities<1, 1, 1>(M1.topo, C1D_1_1);
  }
  bib::message("\n Test 2D connectivities");
  {
    bib::message("C2D_0_0");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 0, 0>(M2.topo, C2D_0_0);
  }
  {
    bib::message("C2D_0_1");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 0, 1>(M2.topo, C2D_0_1);
  }
  {
    bib::message("C2D_0_2");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 0, 2>(M2.topo, C2D_0_2);
  }
  {
    bib::message("C2D_1_0");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 1, 0>(M2.topo, C2D_1_0);
  }
  {
    bib::message("C2D_1_1");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 1, 1>(M2.topo, C2D_1_1);
  }
  {
    bib::message("C2D_1_2");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 1, 2>(M2.topo, C2D_1_2);
  }
  {
    bib::message("C2D_2_0");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 2, 0>(M2.topo, C2D_2_0);
  }
  {
    bib::message("C2D_2_1");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 2, 1>(M2.topo, C2D_2_1);
  }
  {
    bib::message("C2D_2_2");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d.msh");
    test_topology_connectivities<2, 2, 2>(M2.topo, C2D_2_2);
  }
  bib::message("\n Test 3D connectivities");
  {
    bib::message("C3D_0_0");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 0, 0>(M3.topo, C3D_0_0);
  }
  {
    bib::message("C3D_0_1");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 0, 1>(M3.topo, C3D_0_1);
  }
  {
    bib::message("C3D_0_2");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 0, 2>(M3.topo, C3D_0_2);
  }
  {
    bib::message("C3D_0_3");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 0, 3>(M3.topo, C3D_0_3);
  }

  {
    bib::message("C3D_1_0");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 1, 0>(M3.topo, C3D_1_0);
  }
  {
    bib::message("C3D_1_1");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 1, 1>(M3.topo, C3D_1_1);
  }
  {
    bib::message("C3D_1_2");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 1, 2>(M3.topo, C3D_1_2);
  }
  {
    bib::message("C3D_1_3");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 1, 3>(M3.topo, C3D_1_3);
  }

  {
    bib::message("C3D_2_0");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 2, 0>(M3.topo, C3D_2_0);
  }

  {
    bib::message("C3D_2_1");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 2, 1>(M3.topo, C3D_2_1);
  }
  {
    bib::message("C3D_2_2");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 2, 2>(M3.topo, C3D_2_2);
  }
  {
    bib::message("C3D_2_3");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 2, 3>(M3.topo, C3D_2_3);
  }

  {
    bib::message("C3D_3_0");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 3, 0>(M3.topo, C3D_3_0);
  }

  {
    bib::message("C3D_3_1");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 3, 1>(M3.topo, C3D_3_1);
  }

  {
    bib::message("C3D_3_2");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 3, 2>(M3.topo, C3D_3_2);
  }

  {
    bib::message("C3D_3_3");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test2_3d.msh");
    test_topology_connectivities<3, 3, 3>(M3.topo, C3D_3_3);
  }
}

void test_connectivities_hybrid()
{
  bib::message("Test 2D connectivities hybrid");
  {
    bib::message("C2D_0_0 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 0, 0>(M2.topo, C2D_0_0_hybrid);
  }
  {
    bib::message("C2D_0_1 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 0, 1>(M2.topo, C2D_0_1_hybrid);
  }
  {
    bib::message("C2D_0_2 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 0, 2>(M2.topo, C2D_0_2_hybrid);
  }
  {
    bib::message("C2D_1_0 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 1, 0>(M2.topo, C2D_1_0_hybrid);
  }
  {
    bib::message("C2D_1_1 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 1, 1>(M2.topo, C2D_1_1_hybrid);
  }
  {
    bib::message("C2D_1_2 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 1, 2>(M2.topo, C2D_1_2_hybrid);
  }
  {
    bib::message("C2D_2_0 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 2, 0>(M2.topo, C2D_2_0_hybrid);
  }
  {
    bib::message("C2D_2_1 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 2, 1>(M2.topo, C2D_2_1_hybrid);
  }
  {
    bib::message("C2D_2_2 hybrid");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_topology_connectivities<2, 2, 2>(M2.topo, C2D_2_2_hybrid);
  }
  bib::message("Test 3D connectivities hybrid");

  {
    bib::message("C3D_0_0 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 0, 0>(M3.topo, C3D_0_0_hybrid);
  }
  {
    bib::message("C3D_0_1 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 0, 1>(M3.topo, C3D_0_1_hybrid);
  }
  {
    bib::message("C3D_0_2 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 0, 2>(M3.topo, C3D_0_2_hybrid);
  }
  {
    bib::message("C3D_0_3 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 0, 3>(M3.topo, C3D_0_3_hybrid);
  }
  {
    bib::message("C3D_1_0 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 1, 0>(M3.topo, C3D_1_0_hybrid);
  }
  {
    bib::message("C3D_1_1 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 1, 1>(M3.topo, C3D_1_1_hybrid);
  }
  {
    bib::message("C3D_1_2 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 1, 2>(M3.topo, C3D_1_2_hybrid);
  }
  {
    bib::message("C3D_1_3 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 1, 3>(M3.topo, C3D_1_3_hybrid);
  }
  {
    bib::message("C3D_2_0 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 2, 0>(M3.topo, C3D_2_0_hybrid);
  }
  {
    bib::message("C3D_2_1 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 2, 1>(M3.topo, C3D_2_1_hybrid);
  }
  {
    bib::message("C3D_2_2 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 2, 2>(M3.topo, C3D_2_2_hybrid);
  }
  {
    bib::message("C3D_2_3 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 2, 3>(M3.topo, C3D_2_3_hybrid);
  }
  {
    bib::message("C3D_3_0 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 3, 0>(M3.topo, C3D_3_0_hybrid);
  }
  {
    bib::message("C3D_3_1 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 3, 1>(M3.topo, C3D_3_1_hybrid);
  }
  {
    bib::message("C3D_3_2 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 3, 2>(M3.topo, C3D_3_2_hybrid);
  }
  {
    bib::message("C3D_3_3 hybrid");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_topology_connectivities<3, 3, 3>(M3.topo, C3D_3_3_hybrid);
  }
}

#endif