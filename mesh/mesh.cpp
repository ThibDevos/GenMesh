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
  // for(auto c : M1.facets())
  // {
    // for(auto v : c.vertices())
    //   {
    //     std::cout<<v.index()<<" ";
    //   }
    //   std::cout<<"\n";
  // }

  std::cout<<std::endl;
  std::cout<<"========2D========\n";

  std::cout<<M2.topo.nb_vertices()<<" "<<M2.topo.nb_edges()<<" "<<M2.topo.nb_facets()<<" "<<M2.topo.nb_cells()<<"\n";
  std::cout<<"vertices :"<<M2.topo.nb_vertices()<<" \n";
  for(auto c : M2.cells())
  {
    for(auto v : c.vertices())
      {
        std::cout<<v.index()<<" ";
      }
      std::cout<<"\n";
  }
  std::cout<<"inverse"<<std::endl;
  for(auto && v : M2.vertices())
  {
    for(auto && c : v.cells())
    {
      std::cout<<c.index()<<" ";
    }
    std::cout<<"\n";
  }
  std::cout<<"build edges"<<std::endl;
  M2.topo.build_edges();
  auto edge_v  = M2.topo.connectivities[1][0];
  for(auto e : edge_v)
  {
    for(auto v : e)
    {
      std::cout<<v<<" ";
    }
    std::cout<<std::endl;
  }
  std::cout<<"-----------------------"<<std::endl;
  for(auto && e : M2.edges())
  {
      for(auto v : e.vertices())
      {
        std::cout<<v.index()<<" ";
      }
      std::cout<<"\n";
  }
  std::cout<<"fin"<<std::endl;


  std::cout<<std::endl;
  std::cout<<"========3D========\n";

  std::cout<<"vertices :"<<M3.topo.nb_vertices()<<" \n";
  std::cout<<"build edges"<<std::endl;
  M3.topo.build_edges();
  auto edge_v3  = M3.topo.connectivities[1][0];
  for(auto e : edge_v3)
  {
    for(auto v : e)
    {
      std::cout<<v<<" ";
    }
    std::cout<<std::endl;
  }
  std::cout<<"-----------------------"<<std::endl;
  for(auto && e : M3.edges())
  {
      for(auto v : e.vertices())
      {
        std::cout<<v.index()<<" ";
      }
      std::cout<<"\n";
  }
  std::cout<<"fin"<<std::endl;


  std::cout<<std::endl;
  std::cout<<"build facets"<<std::endl;
  M3.topo.build_faces();
  auto face_v  = M3.topo.connectivities[2][0];
  for(auto f : face_v)
  {
    for(auto v : f)
    {
      std::cout<<v<<" ";
    }
    std::cout<<std::endl;
  }
  std::cout<<"-----------------------"<<std::endl;
  for(auto && f : M3.facets())
  {
      for(auto v : f.vertices())
      {
        std::cout<<v.index()<<" ";
      }
      std::cout<<"\n";
  }
  std::cout<<"fin"<<std::endl;

  // for(auto c : M3.facets())
  // {
    // for(auto v : c.vertices())
    //   {
    //     std::cout<<v.index()<<" ";
    //   }
    //   std::cout<<"\n";
  // }

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