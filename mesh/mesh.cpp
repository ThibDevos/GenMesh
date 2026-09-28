#include "parallel/wrapper_mpi.h"
#include <mesh/mesh.h>
#include <io.h>
#include <mpi.h>
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

  int nb_cells;
  if(parallel::is_root())
  {
    nb_cells = M2.topo().nb_cells();
    std::cout<<"nb cells "<<nb_cells<<std::endl;
    std::cout<<"nb vertices "<<M2.topo().nb_vertices()<<std::endl;
  } 

  MPI_Bcast(&nb_cells, 1, MPI_INT, 0, parallel::comm());

  Morton_partition<2> MP;
  mesh<2> M2_loc = MP.partition<2>(M2);
  std::cout<<parallel::rank()<<" has "<<static_cast<double>(M2_loc.topo().nb_cells())/nb_cells*100.<<"\% of the cells" <<std::endl;
  std::cout<<parallel::rank()<<" has "<<static_cast<double>(M2_loc.topo().nb_vertices())<<"vertices" <<std::endl;
  vtu<mesh<2>>::write(M2_loc, "test_files/test_partition");
  return 0;
}