#ifndef TESTS_MESH_CONNECTIVITIES_HYBRID
#define TESTS_MESH_CONNECTIVITIES_HYBRID

#include <vector>
#include <iostream>

using relation_table = std::vector<std::vector<size_t>>;

/*****test_2d_hybrid.msh*****/

const relation_table C2D_2_0_hybrid = {
 {4,9,5}, {2,8,1}, {1,8,7}, {7,9,4}, {8,9,7}, {5,9,2}, {2,9,8}, {0,1,7,6}, {6,7,4,3}
};

const relation_table C2D_0_2_hybrid = {
 {7}, {1,2,7}, {1,5,6}, {8}, {0,3,8}, {0,5}, {7,8}, {2,3,4,7,8}, {1,2,4,6}, {0,3,4,5,6}
};

#endif