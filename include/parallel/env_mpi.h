#ifndef ENV_MPI_H
#define ENV_MPI_H

#if MPI_INSTALLED
#include <mpi.h>

namespace parallel
{
  /*
    Creates an MPI environment for GenMesh.

    The static instance() method ensures that only instance is created.
    The constructor checks if MPI is already initialized and calls MPI_Init if not.

    This implementation is robust if GenMesh is used in a former library calling MPI before using GenMesh.
    In that case, mpi::Environment will not call MPI_Init but it will create its own communicator
  */
  class Environment
  {
    public:
      static Environment& instance()
      {
        static Environment env;
        return env;
      }
      
      int rank() { return rank_;}
      int size() { return size_;}
      MPI_Comm comm() { return comm_;}

    private:
      Environment()
      {
        int initialized;
        MPI_Initialized(&initialized);
        own_mpi_ = !initialized;
        if(own_mpi_)
        {
          MPI_Init(nullptr, nullptr);
        }

        MPI_Comm_dup(MPI_COMM_WORLD, &comm_);
        MPI_Comm_rank(comm_, &rank_);
        MPI_Comm_size(comm_, &size_);

      }
      ~Environment()
      {
        int finilized;
        MPI_Finalized(&finilized);
        if(!finilized)
        {
          if (comm_ != MPI_COMM_WORLD) MPI_Comm_free(&comm_);
          if(own_mpi_) MPI_Finalize();
        }
      }

      bool own_mpi_;
      MPI_Comm comm_;
      int rank_ = 0;
      int size_ = 1;
  };

}

#else
namespace parallel
{
  class Environment
  {
    public:
      static Environment& instance()
      {
        static Environment env;
        return env;
      }
      int rank() {return 0;}
      int size() {return 1;}
  };
}
#endif

#endif