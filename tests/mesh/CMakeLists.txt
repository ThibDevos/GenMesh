include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/tests/mesh")

add_executable(mesh_test main.cpp)

target_compile_definitions(mesh_test PRIVATE 
  TEST_FILES_DIR="${CMAKE_SOURCE_DIR}/test_files")


add_test(Test_mesh, "${CMAKE_SOURCE_DIR}/bin/tests/mesh_test" "0")