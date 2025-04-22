cmake_minimum_required(VERSION 3.10)

project(bib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(CXXFLAGS "-g")

add_subdirectory(include)
add_subdirectory(mesh)

