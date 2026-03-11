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
  mesh<1> M1;
  gmesh<mesh<1>> G;
  G.read_gmsh(M1,"test_files/test_1d.msh");
  mesh<2> M2;
  gmesh<mesh<2>> G2;
  G2.read_gmsh(M2,"test_files/test_2d.msh");
  mesh<3> M3;
  gmesh<mesh<3>> G3;
  G3.read_gmsh(M3,"test_files/test2_3d.msh");



  std::cout<<std::endl;
  std::cout<<"========1D========\n";

  std::cout<<M1.topo.nb_vertices()<<" "<<M1.topo.nb_edges()<<" "<<M1.topo.nb_facets()<<" "<<M1.topo.nb_cells()<<"\n";
  for(auto c : M1.facets())
  {
    // for(auto v : c.vertices())
    //   {
    //     std::cout<<v.index()<<" ";
    //   }
    //   std::cout<<"\n";
  }

  std::cout<<std::endl;
  std::cout<<"========2D========\n";

  std::cout<<M2.topo.nb_vertices()<<" "<<M2.topo.nb_edges()<<" "<<M2.topo.nb_facets()<<" "<<M2.topo.nb_cells()<<"\n";
  std::cout<<"vertices :"<<M2.topo.nb_vertices()<<" \n";
  for(auto c : M2.facets())
  {
    for(auto v : c.cells())
      {
        std::cout<<v.index()<<" ";
      }
      std::cout<<"\n";
  }

  std::cout<<std::endl;
  std::cout<<"========3D========\n";

  std::cout<<M3.topo.nb_vertices()<<" "<<M3.topo.nb_edges()<<" "<<M3.topo.nb_facets()<<" "<<M3.topo.nb_cells()<<"\n";
  std::cout<<"vertices :"<<M3.topo.nb_vertices()<<" \n";

  for(auto c : M3.facets())
  {
    // for(auto v : c.vertices())
    //   {
    //     std::cout<<v.index()<<" ";
    //   }
    //   std::cout<<"\n";
  }

  // for(auto c : M.cells())
  // {
  //   for(auto v : c.vertices())
  //   {
  //     std::cout<<v.index()<<" ";
  //   }
  //   std::cout<<std::endl;
  // }
  return 0;
}