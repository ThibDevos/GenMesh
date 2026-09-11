#include <mesh/mesh.h>
#include <io.h>
#include <parallel/env_mpi.h>

// 2-----3
// |    /|
// |   / |
// |  /  |
// | /   |
// 0-----1
int main()
{

  mpi::Environment::instance();

  mesh<1> M1;
  gmesh<mesh<1>> G;
  G.read_gmsh(M1,"test_files/test_1d.msh");
  mesh<2> M2;
  gmesh<mesh<2>> G2;
  G2.read_gmsh(M2,"test_files/test_2d.msh");
  mesh<3> M3;
  gmesh<mesh<3>> G3;
  G3.read_gmsh(M3,"test_files/test2_3d.msh");

  M3.topo.build_adjacency<1>();
  auto & ad = M3.topo.connectivities[1][1];
  for(int i=0; i<ad.size(); ++i)
  {
    for(int j=0; j<ad[i].size(); ++j)
    {
      std::cout<<ad[i][j]<<" ";
    }
    std::cout<<std::endl;
  }


  for(auto c : M1.cells())
  {
    std::cout<<c.coordinates()[0][0]<<" "<<c.coordinates()[1][0]<<" "<<c.diameter()<<std::endl;
  }
  std::cout<<std::endl;
  for(auto v : M2.vertices())
  {
     std::cout<<v.coordinates()[0][0]<<" "<<v.coordinates()[1][0]<<std::endl;
  }
  return 0;
}