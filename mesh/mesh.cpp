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

  vtu<mesh<3>>::write(M3, "test_files/test_3d.vtu");



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

  // std::cout<<std::endl;
  // std::cout<<"========3D========\n";

  // std::cout<<"vertices :"<<M3.topo.nb_vertices()<<" \n";
  // std::cout<<"build edges"<<std::endl;
  // M3.topo.build_edges();
  // auto edge_v3  = M3.topo.connectivities[1][0];
  // for(auto e : edge_v3)
  // {
  //   for(auto v : e)
  //   {
  //     std::cout<<v<<" ";
  //   }
  //   std::cout<<std::endl;
  // }
  // std::cout<<"-----------------------"<<std::endl;
  // for(auto && e : M3.edges())
  // {
  //     for(auto v : e.vertices())
  //     {
  //       std::cout<<v.index()<<" ";
  //     }
  //     std::cout<<"\n";
  // }
  // std::cout<<"fin"<<std::endl;


  // std::cout<<std::endl;
  // std::cout<<"build facets"<<std::endl;
  // M3.topo.build_faces();
  // auto face_v  = M3.topo.connectivities[2][0];
  // for(auto f : face_v)
  // {
  //   for(auto v : f)
  //   {
  //     std::cout<<v<<" ";
  //   }
  //   std::cout<<std::endl;
  // }
  // std::cout<<"-----------------------"<<std::endl;
  // for(auto && f : M3.facets())
  // {
  //     for(auto v : f.vertices())
  //     {
  //       std::cout<<v.index()<<" ";
  //     }
  //     std::cout<<"\n";
  // }
  // std::cout<<"fin"<<std::endl;

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
  //     {
  //       std::cout<<v.index()<<" ";
  //     }
  //     std::cout<<"\n";
  // }
  // std::cout<<"inverse"<<std::endl;
  // for(auto && v : M2.vertices())
  // {
  //   for(auto && c : v.cells())
  //   {
  //     std::cout<<c.index()<<" ";
  //   }
  //   std::cout<<"\n";
  // }
  // std::cout<<"build edges"<<std::endl;
  // M2.topo.build_edges();
  // auto edge_v  = M2.topo.connectivities[1][0];
  // for(auto e : edge_v)
  // {
  //   for(auto v : e)
  //   {
  //     std::cout<<v<<" ";
  //   }
  //   std::cout<<std::endl;
  // }
  // std::cout<<"-----------------------"<<std::endl;
  // for(auto && e : M2.edges())
  // {
  //     for(auto v : e.vertices())
  //     {
  //       std::cout<<v.index()<<" ";
  //     }
  //     std::cout<<"\n";
  // }
  // std::cout<<"fin"<<std::endl;


  // std::cout<<std::endl;
  // std::cout<<"========3D========\n";

  // std::cout<<"vertices :"<<M3.topo.nb_vertices()<<" \n";
  // std::cout<<"build edges"<<std::endl;
  // M3.topo.build_edges();
  // auto edge_v3  = M3.topo.connectivities[1][0];
  // for(auto e : edge_v3)
  // {
  //   for(auto v : e)
  //   {
  //     std::cout<<v<<" ";
  //   }
  //   std::cout<<std::endl;
  // }
  // std::cout<<"-----------------------"<<std::endl;
  // for(auto && e : M3.edges())
  // {
  //     for(auto v : e.vertices())
  //     {
  //       std::cout<<v.index()<<" ";
  //     }
  //     std::cout<<"\n";
  // }
  // std::cout<<"fin"<<std::endl;


  // std::cout<<std::endl;
  // std::cout<<"build facets"<<std::endl;
  // M3.topo.build_faces();
  // auto face_v  = M3.topo.connectivities[2][0];
  // for(auto f : face_v)
  // {
  //   for(auto v : f)
  //   {
  //     std::cout<<v<<" ";
  //   }
  //   std::cout<<std::endl;
  // }
  // std::cout<<"-----------------------"<<std::endl;
  // for(auto && f : M3.facets())
  // {
  //     for(auto v : f.vertices())
  //     {
  //       std::cout<<v.index()<<" ";
  //     }
  //     std::cout<<"\n";
  // }
  // std::cout<<"fin"<<std::endl;

  // // for(auto c : M3.facets())
  // // {
  //   // for(auto v : c.vertices()) 
  //   //   {
  //   //     std::cout<<v.index()<<" ";
  //   //   }
  //   //   std::cout<<"\n";
  // // }

  // // for(auto c : M.cells())
  // // {
  // //   for(auto v : c.vertices())
  // //   {
  // //     std::cout<<v.index()<<" ";
  // //   }
  // //   std::cout<<std::endl;
  // // }


  for(auto c : M1.cells())
  {
    std::cout<<c.coordinates()[0][0]<<" "<<c.coordinates()[1][0]<<" "<<c.diameter()<<std::endl;
  }
  std::cout<<std::endl;
  for(auto v : M2.vertices())
  {
      std::cout<<v.coordinates()[0][0]<<" "<<v.coordinates()[0][1]<<std::endl;
  }
  return 0;
}