#ifndef WRAPPER_MPI_H
#define WRAPPER_MPI_H

#include <parallel/env_mpi.h>

namespace parallel
{
  inline int rank() {return Environment::instance().rank();}
  inline int size() {return Environment::instance().size();}
  inline MPI_Comm comm() {return Environment::instance().comm();}
  inline bool is_root() {return Environment::instance().rank() == 0;}

}

#endif