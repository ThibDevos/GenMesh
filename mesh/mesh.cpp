#include <mesh/mesh.h>
#include <io.h>
#include <parallel/env_mpi.h>
#include <mesh/partition.h>

// 2-----3
// |    /|
// |   / |
// |  /  |
// | /   |
// 0-----1
int main()
{

  // parallel::Environment::instance();

  mesh<2> M2;
  gmesh<mesh<2>> G2;
  G2.read_gmsh(M2,"test_files/test_2d.msh");
  Morton_partition<2> MP;
  mesh<2> M2_loc = MP.partition<2>(M2);
  
  return 0;
}