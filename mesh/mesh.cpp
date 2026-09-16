#include "parallel/wrapper_mpi.h"
#include <mesh/mesh.h>
#include <io.h>
#include <parallel/env_mpi.h>
#include <mesh/partition.h>
#include <string>

// 2-----3
// |    /|
// |   / |
// |  /  |
// | /   |
// 0-----1
int main()
{

  parallel::Environment::instance();

  mesh<2> M2;
  gmesh<mesh<2>> G2;
  G2.read_gmsh(M2,"test_files/test_2d_for_partition.msh");
  if(parallel::is_root())
    vtu<mesh<2>>::write(M2, "test_files/test_partition_original.vtu");
  std::cout<<"Mesh has "<<M2.topo().nb_vertices()<<" vertices\n";
  Morton_partition<2> MP;
  mesh<2> M2_loc = MP.partition<2>(M2);
  if(parallel::rank()==1)
  {
    for(auto && c : M2_loc.cells())
    {
      std::cout<<c.index();
      for(auto && v : c.vertices())
      {
        std::cout<<"     "<<v.coordinates()[0][0]<<" "<<v.coordinates()[0][1]<<std::endl;
      }
    }
  }
  vtu<mesh<2>>::write(M2_loc, "test_files/test_partition");
  return 0;
}