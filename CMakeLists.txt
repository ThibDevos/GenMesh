cmake_minimum_required(VERSION 3.10)

project(genmesh)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

option(ENABLE_MPI "Compile with MPI" ON)
set(CXXFLAGS "-g")

if(ENABLE_MPI)
  find_package(MPI)
  if(MPI_FOUND)
    add_definitions(-DMPI_INSTALLED)
    link_libraries(MPI::MPI_CXX)
  endif()
endif()

add_subdirectory(include)
add_subdirectory(mesh)


include(CTest)
enable_testing()
add_subdirectory(tests)