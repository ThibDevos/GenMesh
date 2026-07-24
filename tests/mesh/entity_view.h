#ifndef TESTS_MESH_ENTITY_VIEW  
#define TESTS_MESH_ENTITY_VIEW  

#include <expected_data/expected_entity_view.h>


inline bool nearly_equal(double lhs, double rhs)
{
  double scale = std::max({1.0, std::fabs(lhs), std::fabs(rhs)});
  return std::fabs(lhs - rhs) <= 1e-10 * scale;
}

template<size_t G, size_t N, size_t D>
void test_entity_view(mesh<G> M, std::array<expected_entity_view_data<G>,N> expected)
{
  auto entities = M.template entities<D>();
  size_t idx = 0;
  for (auto entity : entities)
  {
    assert(idx < expected.size());
    auto const & expected_value = expected[idx];
    
    assert(entity.nb_vertices() == expected_value.nb_vertices);

    auto coords = entity.coordinates();
    assert(coords.size() == expected_value.coordinates.size());
    for (size_t i = 0; i < coords.size(); ++i)
    {
      for (size_t j = 0; j < G; ++j)
      {
        assert(nearly_equal(coords[i][j], expected_value.coordinates[i][j]));
      }
    }
    auto bary = entity.barycenter();
    for (size_t j = 0; j < G; ++j)
    {
      assert(nearly_equal(bary[j], expected_value.barycenter[j]));
    }

    assert(nearly_equal(entity.diameter(), expected_value.diameter));
    assert(nearly_equal(entity.measure(), expected_value.measure));
    ++idx;
  }

  assert(idx == expected.size());
}

void test_entity_view()
{
   { 
    bib::message("expected_1d_vertices");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_entity_view<1, 5, 0>(M1, expected_1d_vertices);
  }
  { 
    bib::message("expected_1d_edges");
    mesh<1> M1;
    gmesh<mesh<1>> G1;
    G1.read_gmsh(M1, std::string(TEST_FILES_DIR) + "/test_1d.msh");
    test_entity_view<1, 4, 1>(M1, expected_1d_edges);
  }
  { 
    bib::message("expected_2d_hybrid_vertices");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_entity_view<2, 10, 0>(M2, expected_2d_hybrid_vertices);
  }
  { 
    bib::message("expected_2d_hybrid_edges");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_entity_view<2, 18, 1>(M2, expected_2d_hybrid_edges);
  }
  { 
    bib::message("expected_2d_hybrid_cells");
    mesh<2> M2;
    gmesh<mesh<2>> G2;
    G2.read_gmsh(M2, std::string(TEST_FILES_DIR) + "/test_2d_hybrid.msh");
    test_entity_view<2, 9, 2>(M2, expected_2d_hybrid_cells);
  }
  { 
    bib::message("expected_3d_hybrid_vertices");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_entity_view<3, 15, 0>(M3, expected_3d_hybrid_vertices);
  }
  { 
    bib::message("expected_3d_hybrid_edges");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_entity_view<3, 30, 1>(M3, expected_3d_hybrid_edges);
  }
  { 
    bib::message("expected_3d_hybrid_faces");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_entity_view<3, 21, 2>(M3, expected_3d_hybrid_faces);
  }
  { 
    bib::message("expected_3d_hybrid_cells");
    mesh<3> M3;
    gmesh<mesh<3>> G3;
    G3.read_gmsh(M3, std::string(TEST_FILES_DIR) + "/test_3d_hybrid.msh");
    test_entity_view<3, 5, 3>(M3, expected_3d_hybrid_cells);
  } 
}
#endif