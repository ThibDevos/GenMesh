# GenMesh

GenMesh is a lightweight C++ library for managing computational meshes.
It provides a small but expressive API to represent mesh entities, their coordinates,
connectivities, and geometric measures. It is designed as a reusable foundation for
finite element, finite volume, and other numerical simulation workflows.

## Features

- Represent meshes in 1D, 2D, and 3D
- Load meshes from Gmsh `.msh` files
- Iterate over vertices, edges, facets, and cells
- Access geometry through coordinates and barycenters
- Compute simple cell measures
- Explore adjacency through entity-to-entity relationships

## Project structure

- `include/` : public headers and library interfaces
- `mesh/` : sample executable and main entry point
- `tests/` : test cases
- `test_files/` : example mesh files

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

The sample executable is produced at:

```bash
./bin/mesh
```

## Notes

The sample program in `mesh/mesh.cpp` demonstrates loading several meshes and printing
basic mesh information, measures, and adjacency relationships.

