#include <mesh/entity.h>
#include <mesh/mesh.h>
#include <io.h>

// 2-----3
// |    /|
// |   / |
// |  /  |
// | /   |
// 0-----1
int main()
{
  simplex<0> S0;
  simplex<1> S1;
  simplex<2> S2;
  simplex<3> S3;
  simplex<4> S4;
  vertex V;
  entity<simplex,2> E;
  mesh<simplex,3> M;
  read_gmsh(M,"test2_3d.msh");
  for(int i=0; i<M.nb_nodes; ++i)
  {
    M.topo.V[i].print_entity();
    std::cout<<M.geo.coords[i][0]<<" "<<M.geo.coords[i][0]<<std::endl;
  }
  for(int i=0; i<M.nb_cells; ++i)
  {
    M.topo.C[i].print_entity();
  }
  // graph<2,0> gr(M.topo.C[0]);
  // auto g = M.topo.template vertices(M.topo.C[0]);
  for(auto v : vertices(M, M.topo.C[0]))
  {
    std::cout<<v->get_index()<<std::endl;
  }
  for(auto c : cells(M))
  {
    std::cout<<"Cell "<<c.get_index()<<" is composed of vertices" ;
    for(auto v : vertices(M,c))
    {
      std::cout<<v->get_index()<<" ";
    }
    std::cout<<std::endl;
  }
  print_vtu(M,"test.vtu");
  return 0;
}