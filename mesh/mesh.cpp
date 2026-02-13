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
  mesh<3> M;
  gmesh<mesh<3>> G;
  G.read_gmsh(M,"test_files/test2_3d.msh");
  for(auto c : M.cells())
  {
    for(auto v : c.vertices())
    {
      std::cout<<v.index()<<" ";
    }
    std::cout<<std::endl;
  }
  return 0;
}