#include <cassert>
#include <iostream>
#include <vector>

#include <mesh/mesh.h>
#include <io.h>
#include <core/log.h>
#include <connectivities.h>
#include <entity_view.h>
using namespace bib;


int main()
{
  message("Test connectivities");
  test_connectivities();
  message("Test connectivities hybrid");
  test_connectivities_hybrid();
  message("\n Test entity view");
  test_entity_view();
 
  return 0;
}