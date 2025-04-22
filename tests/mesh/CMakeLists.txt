include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(mesh_test main.cpp)

add_test(Test_mesh, "${CMAKE_SOURCE_DIR}/bin/tests/mesh_test" "0")