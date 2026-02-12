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
    std::cout<<c.index()<<std::endl;
  }
  return 0;
}